#!/usr/bin/env python3

from __future__ import annotations

import argparse
import cv2
from pathlib import Path


def extract_frames(video_path: Path, output_dir: Path, desired_fps: float | None):
    output_dir.mkdir(parents=True, exist_ok=True)
    
    cap = cv2.VideoCapture(str(video_path))
    fps = cap.get(cv2.CAP_PROP_FPS)
    fps_ratio = (desired_fps / fps) if desired_fps else 1.0
    
    portion = 0.0
    frame_num = 0
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        portion += fps_ratio
        if portion >= 1.0:
            portion -= 1.0
            cv2.imwrite(str(output_dir / f"{frame_num:05d}.jpg"), frame)
            frame_num += 1
    
    cap.release()
    print(f"Extracted {frame_num} frames")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output-dir", type=Path, required=True)
    parser.add_argument("-v", "--video-path", type=Path, required=True)
    parser.add_argument("--desired-fps", type=float, default=60.0)
    args = parser.parse_args()
    extract_frames(args.video_path, args.output_dir, args.desired_fps)


if __name__ == "__main__":
    main()
