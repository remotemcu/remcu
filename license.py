import argparse
import hashlib

parser = argparse.ArgumentParser(description='License client')

parser.add_argument(
    "-int", help="file text license", default='LICENSE.txt')

parser.add_argument("-out", help="fiel for lib", default='REMCU_LICENSE.txt')

parser.add_argument("-t", help="type", default='UNKNOW')

args = parser.parse_args()


IN_TEXT_LICENSE_FILE = args.int
OUT_LECENSE_ASIGN = args.out
TYPE = args.t

print(IN_TEXT_LICENSE_FILE, OUT_LECENSE_ASIGN)


if TYPE == 'EDU' :
	TYPE = 'EDU'
elif TYPE == 'COM':
	TYPE = 'COM'
else :
	raise Exception('unknow license')


text = ''

with open(IN_TEXT_LICENSE_FILE, 'r') as inFile:
	text = inFile.read()

text_trim = text.replace('\n', '')

text_trim += TYPE

HASH_LICENSE = hashlib.sha256(text_trim.encode('utf-8')).hexdigest()

print(">", HASH_LICENSE)

with open(OUT_LECENSE_ASIGN, 'w+') as outFile:
	outFile.write(text)
	outFile.write("#" + HASH_LICENSE)
	outFile.write("\n")
	outFile.flush()
