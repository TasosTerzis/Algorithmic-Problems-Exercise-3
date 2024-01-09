import matplotlib.pyplot as plt
import keras as kr
import numpy as np
import argparse
import struct

def read_mnist_images(file_path):
    with open(file_path, 'rb') as f:
        magic_number, num_images, rows, cols = struct.unpack('>IIII', f.read(16))
        image_size = rows * cols
        images = np.frombuffer(f.read(image_size * num_images), dtype=np.uint8)
        images = images.reshape(num_images, rows, cols)
    return images, magic_number, num_images, rows, cols

def process_file(input_file, output_file):
    dataset, d_magic_number, d_num_images, d_rows, d_cols = read_mnist_images(input_file)
    
    encoder = kr.models.load_model('./models/encoder.h5')
    
    dataset = dataset.reshape(-1, 28, 28, 1)
    dataset = dataset.astype('float32') / 255.0
    
    output_dataset = encoder.predict(dataset)
    output_dataset = (output_dataset * 255).astype('uint8')
    
    with open(output_file, 'wb') as f:
        f.write(struct.pack('>IIII', d_magic_number, d_num_images, 28, 1))
        f.write(output_dataset.tobytes())

# command line looks like: python reduce.py -d ../input/10k_images.dat -q ../input/100from60k.dat -od ../input/10k_images_REDUCED.dat -oq ../input/100from60k_REDUCED.dat
# read in command line arguments
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--dataset', help='dataset file', required=False)
parser.add_argument('-q', '--queryset', help='queryset file', required=False)
parser.add_argument('-od', '--output_dataset_file',
                    help='output dataset file', required=False)
parser.add_argument('-oq', '--output_query_file',
                    help='output query file', required=False)
args = parser.parse_args()

# call process file function, for both dataset and query set if they exist
if args.dataset:
    process_file(args.dataset, args.output_dataset_file)
if args.queryset:
    process_file(args.queryset, args.output_query_file)
    