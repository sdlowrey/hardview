#
# illustrate a python technique to read smbios
#
import struct
import sys
from collections import namedtuple

Header = namedtuple('Header', 'type len handle')
Bios = namedtuple('Bios', 'vendor version startaddr reldate romsz chars ext1 ext2 relmaj relmin')
System = namedtuple('System', 'mfr prod ver serial uuid wake sku fam')

def findEntryPoint(path):
    try:
        f = open(path, 'rb')
    except:
        print 'open %s failed' % path
        return False
    
    offset = 0
    if path == '/dev/mem':
        f.seek(0xf0000)
        buf = f.read(0x10000)
        offset = buf.find('_SM_') + 0xf0000
    f.seek(offset)
    return (f, f.read(0x1f))

def parseEntryPoint(buf):
    epStruct = struct.unpack_from('4c4bhb5b5cbhihb', buf)
    
    epMap = {
        'vermajor': epStruct[6],
        'verminor': epStruct[7],
        'length':   epStruct[21],
        'address':  epStruct[22],
        'scount':   epStruct[23],
        }
    print 'SMBIOS Version: %d.%d' % (epMap['vermajor'], epMap['verminor'])
    print 'Table is %d structures in %d bytes starting at %d' % (
        epMap['scount'], epMap['length'], epMap['address'])
    return epMap

def getTable(f, ep):
    f.seek(ep['address'])
    return f.read(ep['length'])

def toHeader(s):
    return struct.unpack_from('2BH', s)
   
def toString(strs, idx):
    return strs[idx-1]

def parseStruct(_struct, _str):
    h = toHeader(_struct)
    print 'Type: %d  Length: %d  Handle: %04x' %  h
    print 'Strings: ' + ", ".join(_str)
    if h[0] == 0:
        fmt = '2B H 2B L 2B 2B'
        print "Format Size: ", struct.calcsize(fmt)
        b = Bios._make(struct.unpack_from(fmt, _struct[4:]))
        print toString(_str, b.vendor), toString(_str, b.version), toString(_str, b.reldate) 

def parseTable(ep, table):
    typesWanted = set([0,1,7])
    typesFound = set()
    for i in range(ep['scount']):
        h = toHeader(table)
        _type = h[0]
        length = h[1]
        _struct = table[:length]
        table = table[length:]
        end = table.find('\x00\x00')+2
        _strings = table[:end].split('\x00')
        # shave off empty items created by double-null at end of string list
        _strings = _strings[:len(_strings)-2]
        table = table[end:]
        if _type in typesWanted:
            typesFound.add(_type)
            parseStruct(_struct, _strings)
    print 'found wanted types', typesFound
       
        
if __name__ == '__main__':
    inpath = '/dev/mem'
    if len(sys.argv) > 1:
        inpath = sys.argv[1]
    infile, rawEP = findEntryPoint(inpath)
    mappedEP = parseEntryPoint(rawEP)
    table = getTable(infile, mappedEP)
    parseTable(mappedEP, table)
    infile.close()
    
    
