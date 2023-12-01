import random
import string

def generate_mixed_chars(num_chars):
	safe_ascii_chars = string.ascii_letters + string.digits + "-_."
	unicode_chars = ''.join(chr(i) for i in range(0x0400, 0x0450))
	combined_chars = safe_ascii_chars + unicode_chars
	return ''.join(random.choice(combined_chars) for _ in range(num_chars))

def write_to_file(filename, content):
	with open(filename, 'w', encoding='utf-8') as file:
		file.write(content)

num_chars = 100000
mixed_chars = generate_mixed_chars(num_chars)
write_to_file('test1.txt', mixed_chars)

