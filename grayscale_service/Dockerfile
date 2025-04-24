# Use Ubuntu 22.04 as base
FROM ubuntu:22.04

# Prevent prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies (OpenCV + C++ build tools)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Create a directory for the executable
RUN mkdir -p /usr/local/bin/grayscale

# Download face cascade classifier
RUN mkdir -p /usr/share/opencv4/haarcascades && \
    wget https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml \
    -O /usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml

# Copy the entire grayscale_service folder contents
COPY . .

# Build the C++ app
RUN mkdir -p build && cd build \
    && cmake .. \
    && make \
    && cp grayscale_service /usr/local/bin/grayscale/ \
    && chmod +x /usr/local/bin/grayscale/grayscale_service \
    && cd .. \
    && rm -rf build

# Expose port 80
EXPOSE 80

# Default command to run the grayscale filter
CMD ["/usr/local/bin/grayscale/grayscale_service"]
