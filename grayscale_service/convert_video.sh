#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 input_video output_video"
    echo "Example: $0 input.mp4 output.avi"
    exit 1
fi

INPUT_VIDEO=$1
OUTPUT_VIDEO=$2

if [ ! -f "$INPUT_VIDEO" ]; then
    echo "Error: Input video file '$INPUT_VIDEO' does not exist"
    exit 1
fi

echo "Converting $INPUT_VIDEO to grayscale..."
curl -X POST -F "video=@$INPUT_VIDEO" http://localhost:8080/process --output "$OUTPUT_VIDEO"

if [ $? -eq 0 ]; then
    echo "Conversion complete! Output saved as $OUTPUT_VIDEO"
else
    echo "Error during conversion"
    exit 1
fi 