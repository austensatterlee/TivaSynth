import numpy as np
from numpy import linspace,logspace,arange,int32
from numpy import log2,sin,pi,floor,ceil

FS = 192000
MOD_FS = FS>>8
Q = 31
TABLESIZE = 256

class Table:
    def __init__(self,table,name,longname="",commentinfo={},cellfmt='#08x'):
        self.longname = longname
        self.table = table
        self.size = len(table)
        self.name = name
        self.ctype = 'q31_t' if Q==31 else 'q15_t'
        self.cellfmt = cellfmt
        self.commentinfo = commentinfo
        self.commentinfo.update({'FS':FS,'MOD_FS':MOD_FS,'Q':Q})
        self.comment = """/*
 * {}
 *""".format(self.longname)
        for k in sorted(self.commentinfo.keys()):
            self.comment+='\n * {}: {}'.format(k,self.commentinfo[k])
        self.comment+='\n *\n */\n'
    def showc(self):
        mystr = self.comment
        mystr += "const {} {}[{}] = {{\n".format(
                self.ctype,self.name,len(self.table))
        mystr += self.__str__()
        return mystr+'\n};\n'
    def __str__(self):
        rows=int( np.sqrt(self.size) )
        cols=int( ceil(rows) )
        cellstr="{:%s}"%self.cellfmt
        showstr=""
        ind=0
        for i in xrange(rows):
            if i!=0:
                showstr+='\n'
            for j in xrange(cols):
                showstr+=cellstr.format(self.table[ind])
                showstr+=','
                ind+=1
                if ind>self.size:
                    break
            if ind>self.size:
                break
        return showstr

def GenEnvAlphas(t0,t1,tablesize=None,res=None):
    envfs       = FS*1.0/MOD_FS
    mintimeres  = 1.0/envfs
    t0 = max(t0,mintimeres)
    if tablesize:
        timeres = max(mintimeres,float(t1-t0)/tablesize)
    elif res:
        timeres = max(mintimeres,res)
    else:
        timeres = float(t1-t0)/TABLESIZE
    tablesize = int(float(t1-t0)/timeres)
    times     = linspace(t0,t1,tablesize)

    alphaTable = np.int32(0.05**(1./np.floor(envfs*times))*(1<<31))
    return Table(alphaTable,
            'envDecayTable',
            'Envelope decay map',
            {'t0':t0,'t1':t1}
            )

def GenFilterCutoffs(f0,f1,tablesize=None,res=None):
    f1 = min(f1,FS>>1)
    if not tablesize:
        if res:
            tablesize = (f1-f0)/res
        else:
            tablesize = TABLESIZE
    F = logspace(log2(f0),log2(f1),tablesize,base=2,endpoint=False)
    fcTable = int32(sin(2*pi*F/FS)*(1<<31))
    return Table(fcTable,
            'fcTable',
            'Filter cutoff map',
            {'f0':f0,'f1':f1,}
            )

def GenOscFreqs(f0,tablesize=None):
    if not tablesize:
        tablesize = TABLESIZE
    freqs = f0*(2**(arange(0,tablesize)/12.0))
    freqTable = int32(freqs*(1<<31)/FS)
    return Table(freqTable,
            'noteTable',
            'Oscillator phase step map (notes)',
            {'f0':f0}
            )

def GenOscSteps(tablesize=None):
    if not tablesize:
        tablesize = TABLESIZE
    freqsteps = 2**(arange(-tablesize/2,tablesize/2)/12.0)
    freqstepTable = int32(freqsteps*(1<<31)/FS)
    return Table(
            freqstepTable,
            'semitoneTable',
            'Oscillator phase step map (semitones)',
            )

if __name__=='__main__':
    import argparse
    parser=argparse.ArgumentParser()
    parser.add_argument('--outfile',type=str,default="tabledata.c")
    args=parser.parse_args()
    tablecfile = "#include \"arm_math.h\"\n"
    tables=[
    GenEnvAlphas(0.005,5.0),
    GenFilterCutoffs(100,80000),
    GenOscFreqs(440*2**(-14)),
    GenOscSteps(),
    ]
    for table in tables:
        tablecfile+="\n"+table.showc()
    with open(args.outfile,'w') as outfp:
        outfp.write(tablecfile)
