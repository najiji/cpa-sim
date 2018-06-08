//
// Created by najiji on 19.05.18.
//
#include <iostream>
#include "sim.h"

#include <mutex>
#include <string>
#include "opencv2/opencv.hpp"
#include <chrono>


#define MIN_PERIOD 32
#define N_FPS_AVERAGE 10

// -- Grid layout definition for the window alignments
#define TILE_WIDTH 256
#define TILE_HEIGHT 280
#define TILE_COUNT_H 2

using namespace std;

Sim::Sim() :
        last_frame_download(chrono::high_resolution_clock::now()) {
    cout << cv::getBuildInformation() << std::endl;
}

Sim::~Sim() {
    free(cap);
}

void Sim::start_ui () {
    for (int i=0; i<display_images.size(); i++) {
        cv::namedWindow(window_names[i], cv::WINDOW_AUTOSIZE);
        cv::moveWindow(window_names[i], i%TILE_COUNT_H*TILE_WIDTH, i/TILE_COUNT_H*TILE_HEIGHT);
    }
}

void Sim::acquire_frame() {
    if (cap == nullptr) {
        std::cerr << "[Error] No capture source specified" << std::endl;
    }
    *cap >> frame;
}

void Sim::update_ui() {
    auto current_time = chrono::high_resolution_clock::now();
    long time_since_last_frame = chrono::duration_cast<chrono::milliseconds>(current_time - last_frame_invocation).count();
    fps = fps * (N_FPS_AVERAGE-1)/N_FPS_AVERAGE + (1000./time_since_last_frame)/N_FPS_AVERAGE;
    cout << fps << " fps\r";
    last_frame_invocation = current_time;
    if (current_time - last_frame_download >= chrono::milliseconds(MIN_PERIOD)) {
        lock_guard<mutex> l(download_guard);
        last_frame_download = chrono::high_resolution_clock::now();
        for (int i=0; i<gpu_images.size(); i++) {
            gpu_images[i].copyTo(display_images[i]);
        }
        for (int i=0; i<display_images.size(); i++) {
            cv::imshow(window_names[i], display_images[i]);
            cv::waitKey(1);
        }
    }
}

void Sim::source_camera() {
    cap = new cv::VideoCapture(0);
}

void Sim::source_video(const string& filename) {
    cap = new cv::VideoCapture(filename);
}

const cv::Mat &Sim::get_frame() const {
    return frame;
}

void Sim::add_window(const cv::UMat& reg) {
    add_window(reg, "Output_" + to_string(window_names.size()));
}

void Sim::add_window(const cv::UMat& reg, const string& name) {
    gpu_images.push_back(reg);
    window_names.push_back(name);
    cv::Mat display_image(reg.size(), reg.type());
    {
        lock_guard<mutex> l(download_guard);
        display_images.push_back(display_image);
    }
}


