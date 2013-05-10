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
        'tablen',
        'tabaddr',
        'numstrucs']
#        'bcdrev']
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
        self._parseEntryPoint(buf)

    def _parseEntryPoint(self, buf):
        epFmt = '=4s 4B H B 5s 5s B H I H'
        #print 'epFmt sz: %x' % struct.calcsize(epFmt)
        ep = struct.unpack_from(epFmt, buf)
        self._entryPoint = EntryPoint._make(ep)

    def parse(self):
        self._getEntryPoint()
        #self._getTable()
        #self._parseTable()
        self._cleanup()

    def briefTextSummary(self):
        return 'Input File: {}\nSMBIOS Version: {}.{}'.format(
            self.path, 
            self._entryPoint.vermaj,
            self._entryPoint.vermin) 

if __name__ == '__main__':
    inpath = '/dev/mem'
    if len(sys.argv) > 1:
        inpath = sys.argv[1]
    s = SmBios(inpath)
    s.parse()
#    except:
#        print "smbios err:", sys.exc_info()[0]
    print s
