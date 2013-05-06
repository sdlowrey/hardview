#
# illustrate a python technique to read smbios
#
import struct
import sys


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

if __name__ == '__main__':
    inpath = '/dev/mem'
    if len(sys.argv) > 1:
        inpath = sys.argv[1]
    infile, rawEP = findEntryPoint(inpath)
    mappedEP = parseEntryPoint(rawEP)
    table = getTable(infile, mappedEP)
    
    
