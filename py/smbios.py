import struct
import sys
from collections import namedtuple

Header = namedtuple(
    'Header', [
        'type',
        'len',
        'handle']
    )

EntryPoint = namedtuple(
    'EntryPoint', [
        'anchor',
        'cksum',
        'eplen',
        'vermaj',
        'vermin',
        'maxstrucsz',
        'rev',
        'fmtd',
        'ianchor',
        'icksum',
        'tlen',
        'taddr',
        'numstrucs']
#        'bcdrev']
    )

Bios = namedtuple(
    'Bios', [
        'vendor', 
        'version', 
        'startaddr',
        'reldate',
        'romsz',
        'chars', 
        'ext1', 
        'ext2', 
        'relmaj',
        'relmin']
    )

class SmBios:
    def __init__(self, path):
        self.path = path
        self._in = open(path, 'rb')
        self._entryPoint = None
        self._table = None

    def __str__(self):
        return self.briefTextSummary()

    def __del__(self):
        self._cleanup()

    def _cleanup(self):
        self._in.close()

    def _peek(self, bytes):
        prevOffset = self._in.tell()
        buf = self._in.read(bytes)
        self._in.seek(prevOffset)
        return buf

    def _getEntryPoint(self):
        offset = 0
        if self.path == '/dev/mem':
            self._in.seek(0xf0000)
            buf = self._in.read(0x10000)
            offset = buf.find('_SM_') + 0xf0000
        self._in.seek(offset)
        anchor = self._peek(4)
        if anchor != '_SM_':
            raise Exception('Unable to find SMBIOS Entry Point')
        buf = self._in.read(0x1f)

        epFmt = '=4s 4B H B 5s 5s B H I H'
        #print 'epFmt sz: %x' % struct.calcsize(epFmt)
        ep = struct.unpack_from(epFmt, buf)
        self._entryPoint = EntryPoint._make(ep)

    def parse(self):
        self._getEntryPoint()
        self._table = SmBiosTable(self._entryPoint, self._in)
        self._cleanup()

    def briefTextSummary(self):
        return 'Input File: {}\nSMBIOS Version: {}.{}'.format(
            self.path, 
            self._entryPoint.vermaj,
            self._entryPoint.vermin) 

    def dumpHeaders(self):
        for h in self._table:
            print h

class SmBiosTable:
    def __init__(self, entryPoint, infile):
        self._in = infile
        infile.seek(entryPoint.taddr)
        self._table = infile.read(entryPoint.tlen)
        self._nStructs = entryPoint.numstrucs
        self._count = 0
        self._offset = 0

    def __iter__(self,):
        return self

    def next(self):
        if self._count == self._nStructs:
            raise StopIteration

    def _parseStruct(self, hdr,  struc, strng):
        # unfinished
        fmt = StructureFormat(hdr.type)
        # instead of...
        fmt = '2B H 2B L 2B 2B'
        bios = Bios._make(struct.unpack_from(fmt, sbuf))
        print bios

if __name__ == '__main__':
    inpath = '/dev/mem'
    if len(sys.argv) > 1:
        inpath = sys.argv[1]
    s = SmBios(inpath)
    s.parse()
    s.dumpHeaders()
#    except:
#        print "smbios err:", sys.exc_info()[0]
    print s
