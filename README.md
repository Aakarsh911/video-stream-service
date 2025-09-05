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
