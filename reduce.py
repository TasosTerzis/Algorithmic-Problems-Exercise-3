# code to reduce dimension of images
import keras as kr
import numpy as np
import pandas as pd
import tensorflow as tf
import argparse
import struct

# command line looks like: python reduce.py –d <dataset> -q <queryset> -od <output_dataset_file> -oq <output_query_file>
# read in command line arguments
parser = argparse.ArgumentParser()
parser.add_argument('-d', '--dataset', help='dataset file', required=True)
parser.add_argument('-q', '--queryset', help='queryset file', required=True)
parser.add_argument('-od', '--output_dataset_file', help='output dataset file', required=True)
parser.add_argument('-oq', '--output_query_file', help='output query file', required=True)
args = parser.parse_args()

# make a function to get the MNIST data from a file and return it as a numpy array
def read_mnist_images(file_path):
    with open(file_path, 'rb') as f:
        # Read magic number, number of images, rows, and columns
        magic_number, num_images, rows, cols = struct.unpack('>IIII', f.read(16))

        # Read image data
        image_size = rows * cols
        images = np.frombuffer(f.read(image_size * num_images), dtype=np.uint8)
        images = images.reshape(num_images, rows, cols)

    return images, magic_number, num_images, rows, cols

# Read input files
dataset, d_magic_number, d_num_images, d_rows, d_cols = read_mnist_images(args.dataset)
queryset, q_magic_number, q_num_images, q_rows, q_cols = read_mnist_images(args.queryset)

# load the encoder model
encoder = kr.models.load_model('encoder.h5')