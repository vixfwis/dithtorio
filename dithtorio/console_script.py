from libdithtorio import process_file
import sys
import os

def main():
	if len(sys.argv) < 2:
		print('Usage: dithtorio sourceImageFile')
		return
	path, ext = os.path.splitext(sys.argv[1])
	head, tail = os.path.split(path)
	process_file(
		sys.argv[1],
		os.path.join(head, 'converted_'+tail+'.png'),
		os.path.join(head, 'blueprint_'+tail+'.txt')
	)
