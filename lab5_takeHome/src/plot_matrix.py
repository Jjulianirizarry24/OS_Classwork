import csv
import numpy as np

import matplotlib  # comment for MACOS

matplotlib.use("TkAgg")  # comment for MACOS

import matplotlib.pyplot as plt
import sys
import os


def plot_matrix_from_csv(csv_filepath, output_filepath="matrix_plot.png"):
    """
    Reads a matrix from a CSV file and plots it as a black and white image.

    Args:
        csv_filepath (str): The path to the CSV file containing the matrix data.
                          Each row in the CSV should represent a row in the matrix,
                          with values separated by commas.
        output_filepath (str, optional): The path to save the output image.
                                         Defaults to "matrix_plot.png".
    """
    try:
        if not os.path.exists(csv_filepath):
            raise FileNotFoundError(f"CSV file not found at '{csv_filepath}'")

        matrix = []
        with open(csv_filepath, "r", newline="") as csvfile:
            reader = csv.reader(csvfile)
            for row in reader:
                try:
                    matrix.append([float(val) for val in row])
                except ValueError:
                    raise ValueError(
                        "Could not convert data in CSV file to numbers. Ensure all entries are numeric."
                    )

        if not matrix:
            print("Warning: CSV file is empty.")
            return

        # Convert the list of lists to a NumPy array
        matrix_array = np.array(matrix)

        # Ensure the values are within the range [0, 1] for black and white
        # You might need to normalize your data if it's outside this range
        # Example normalization:
        # matrix_array = (matrix_array - np.min(matrix_array)) / (np.max(matrix_array) - np.min(matrix_array))

        # Display the matrix as an image
        plt.imshow(matrix_array, cmap="gray", vmin=0, vmax=1)
        plt.axis("off")  # Turn off axis labels and ticks

        # Save the plot to a file
        plt.savefig(output_filepath)
        plt.show()  # Optional: Display the plot
        print(
            f"Matrix from '{os.path.basename(csv_filepath)}' plotted and saved to '{output_filepath}'"
        )

    except FileNotFoundError as e:
        print(f"Error: {e}")
    except ValueError as e:
        print(f"Error: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_matrix.py <csv_filename> [output_filename]")
        sys.exit(1)

    csv_file = sys.argv[1]  # Expecting filename as the second argument
    print(csv_file)
    output_file = "matrix_plot.png"  # Default output name

    if len(sys.argv) >= 3:
        output_file = sys.argv[2]  # Optional output filename as the third argument

    plot_matrix_from_csv(csv_file, output_file)
