#include <httplib.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace cv;
using namespace std;

// Grayscale filter
bool convertToGrayscale(const string& inputPath, const string& outputPath) {
    VideoCapture cap(inputPath);
    if (!cap.isOpened()) return false;

    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);

    VideoWriter video(outputPath, VideoWriter::fourcc('M','J','P','G'), fps, Size(frame_width, frame_height), false);

    Mat frame;
    while (cap.read(frame)) {
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        video.write(gray);
    }

    cap.release();
    video.release();
    return true;
}

// Face blurring filter
bool blurFaces(const string& inputPath, const string& outputPath) {
    VideoCapture cap(inputPath);
    if (!cap.isOpened()) return false;

    CascadeClassifier face_cascade;
    if (!face_cascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
        cerr << "Error loading face cascade\n";
        return false;
    }

    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);

    VideoWriter video(outputPath, VideoWriter::fourcc('M','J','P','G'), fps, Size(frame_width, frame_height), true);

    Mat frame;
    while (cap.read(frame)) {
        vector<Rect> faces;
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        face_cascade.detectMultiScale(gray, faces);

        for (const Rect& face : faces) {
            GaussianBlur(frame(face), frame(face), Size(35, 35), 0);
        }

        video.write(frame);
    }

    cap.release();
    video.release();
    return true;
}

int main() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
    });

    svr.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_file("video")) {
            res.status = 400;
            res.set_content("No video file provided", "text/plain");
            return;
        }

        auto video = req.get_file_value("video");
        string inputPath = "/app/temp_input.mp4";
        string outputPath = "/app/temp_output.avi";

        ofstream ofs(inputPath, ios::binary);
        ofs.write(video.content.c_str(), video.content.size());
        ofs.close();

        auto start = chrono::high_resolution_clock::now();
        bool success = convertToGrayscale(inputPath, outputPath);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "[/process] Grayscale conversion took " << duration << " ms" << endl;

        if (success) {
            ifstream ifs(outputPath, ios::binary);
            string output_content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
            ifs.close();
            res.set_content(output_content, "video/avi");
        } else {
            res.status = 500;
            res.set_content("Failed to process video", "text/plain");
        }

        remove(inputPath.c_str());
        remove(outputPath.c_str());
    });

    svr.Post("/blurfaces", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_file("video")) {
            res.status = 400;
            res.set_content("No video file provided", "text/plain");
            return;
        }

        auto video = req.get_file_value("video");
        string inputPath = "/app/temp_input.mp4";
        string outputPath = "/app/temp_output.avi";

        ofstream ofs(inputPath, ios::binary);
        ofs.write(video.content.c_str(), video.content.size());
        ofs.close();

        auto start = chrono::high_resolution_clock::now();
        bool success = blurFaces(inputPath, outputPath);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "[/blurfaces] Face blurring took " << duration << " ms" << endl;

        if (success) {
            ifstream ifs(outputPath, ios::binary);
            string output_content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
            ifs.close();
            res.set_content(output_content, "video/avi");
        } else {
            res.status = 500;
            res.set_content("Failed to blur faces", "text/plain");
        }

        remove(inputPath.c_str());
        remove(outputPath.c_str());
    });

    cout << "🚀 Server running on port 80..." << endl;
    svr.listen("0.0.0.0", 80);
    return 0;
}
