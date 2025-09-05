# Video Stream Service

A high-performance C++ microservice for video processing with OpenCV, providing grayscale conversion and face blurring capabilities through REST APIs.

## Features

- **Grayscale Conversion**: Convert color videos to grayscale format
- **Face Blurring**: Automatically detect and blur faces in videos using Haar cascade classifiers
- **REST API**: Simple HTTP endpoints for video processing
- **Docker Support**: Containerized deployment ready
- **Kubernetes Ready**: Production-ready Kubernetes manifests included
- **Performance Monitoring**: Built-in timing metrics for operations

## Architecture

The service is built with:
- **C++ & OpenCV**: Core video processing engine
- **cpp-httplib**: Lightweight HTTP server framework
- **Docker**: Containerization for consistent deployment
- **Kubernetes**: Orchestration and scaling support

## API Endpoints

### Health Check
```
GET /health
```
Returns service health status.

**Response:**
```
OK
```

### Grayscale Conversion
```
POST /process
```
Converts uploaded videos to grayscale format.

**Parameters:**
- `video` (file): Video file to process (multipart/form-data)

**Response:**
- Success: Grayscale video file (video/avi)
- Error: Error message with appropriate HTTP status code

**Example:**
```bash
curl -X POST -F "video=@input.mp4" http://localhost:8080/process --output grayscale_output.avi
```

### Face Blurring
```
POST /blurfaces
```
Detects and blurs faces in uploaded videos using computer vision.

**Parameters:**
- `video` (file): Video file to process (multipart/form-data)

**Response:**
- Success: Video with blurred faces (video/avi)
- Error: Error message with appropriate HTTP status code

**Example:**
```bash
curl -X POST -F "video=@input.mp4" http://localhost:8080/blurfaces --output blurred_output.avi
```

## Quick Start

### Using Docker (Recommended)

1. **Build the Docker image:**
```bash
cd grayscale_service
docker build -t video-stream-service .
```

2. **Run the container:**
```bash
docker run -p 8080:80 video-stream-service
```

3. **Test the service:**
```bash
# Health check
curl http://localhost:8080/health

# Process a video (replace with your video file)
curl -X POST -F "video=@your_video.mp4" http://localhost:8080/process --output result.avi
```

### Using Convenience Scripts

The repository includes helper scripts for easy video processing:

**Grayscale conversion:**
```bash
cd grayscale_service
chmod +x convert_video.sh
./convert_video.sh input.mp4 output_grayscale.avi
```

**Face blurring:**
```bash
cd grayscale_service
chmod +x blur_faces.sh
./blur_faces.sh input.mp4 output_blurred.avi
```

## Local Development

### Prerequisites
- Ubuntu 22.04 or compatible Linux distribution
- CMake 3.10+
- OpenCV development libraries
- C++ compiler with C++11 support
- Git

### Build from Source

1. **Install dependencies:**
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake git wget unzip libopencv-dev
```

2. **Download face detection model:**
```bash
sudo mkdir -p /usr/share/opencv4/haarcascades
sudo wget --no-check-certificate https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml \
    -O /usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml
```

3. **Build the service:**
```bash
cd grayscale_service
mkdir build && cd build
cmake ..
make
```

4. **Run the service:**
```bash
./grayscale_service
```

## Kubernetes Deployment

Deploy to a Kubernetes cluster using the provided manifests:

```bash
cd grayscale_service/k8s
kubectl apply -f deployment.yaml
```

This creates:
- **Deployment**: 2 replicas with resource limits (512Mi memory, 500m CPU)
- **Service**: ClusterIP service exposing port 80
- **Storage**: EmptyDir volume for temporary video processing

### Accessing the Service

After deployment, access the service within your cluster:
```bash
# Port forward for local access
kubectl port-forward service/grayscale-service 8080:80

# Or create an ingress/load balancer as needed
```

## Performance

The service includes built-in performance monitoring:
- Processing times are logged for each operation
- Temporary files are automatically cleaned up after processing
- Memory-efficient streaming processing for large video files

Example log output:
```
🚀 Server running on port 80...
[/process] Grayscale conversion took 1250 ms
[/blurfaces] Face blurring took 2100 ms
```

## Supported Video Formats

- **Input**: MP4, AVI, MOV, and other OpenCV-supported formats
- **Output**: AVI format with MJPEG codec

## Error Handling

The service provides clear error responses:
- `400 Bad Request`: No video file provided
- `500 Internal Server Error`: Video processing failed

## Resource Requirements

### Minimum Requirements
- CPU: 250m (0.25 cores)
- Memory: 256Mi
- Storage: Ephemeral storage for video processing

### Production Recommendations
- CPU: 500m+ (0.5+ cores)
- Memory: 512Mi+
- Storage: Fast SSD for temporary file operations

## Limitations

- Maximum video file size is limited by available memory
- Face detection works best with frontal faces
- Processing time scales with video length and resolution
- Temporary files require disk space equal to ~2x input video size during processing

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.