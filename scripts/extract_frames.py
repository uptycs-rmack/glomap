#!/usr/bin/env python3

from __future__ import annotations

import argparse
import cv2
from pathlib import Path


def extract_frames(video_path: Path, output_dir: Path, desired_fps: float | None):
    output_dir.mkdir(parents=True, exist_ok=True)

    cap = cv2.VideoCapture(str(video_path))
    fps = cap.get(cv2.CAP_PROP_FPS)
    total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    fps_ratio = (desired_fps / fps) if desired_fps else 1.0

    # Estimate how many frames will be extracted
    estimated_output = int(total_frames * fps_ratio) if fps_ratio < 1.0 else total_frames

    print(f"Video: {video_path.name}")
    print(f"FPS: {fps:.2f}, Total frames: {total_frames}, Target FPS: {desired_fps or fps:.2f}")
    print(f"Estimated output frames: ~{estimated_output}")

    portion = 0.0
    frame_num = 0
    input_frame_num = 0
    last_percent = -1

    while True:
        ret, frame = cap.read()
        if not ret:
            break
        input_frame_num += 1
        portion += fps_ratio
        if portion >= 1.0:
            portion -= 1.0
            cv2.imwrite(str(output_dir / f"{frame_num:05d}.jpg"), frame)
            frame_num += 1

        # Print progress every 1%
        percent = int(100 * input_frame_num / total_frames) if total_frames > 0 else 0
        if percent != last_percent:
            print(f"\rProgress: {percent}% ({frame_num} frames extracted)", end="", flush=True)
            last_percent = percent

    print(f"\rProgress: 100% - Done!                    ")
    cap.release()
    print(f"Extracted {frame_num} frames to {output_dir}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-o", "--output-dir", type=Path, required=True)
    parser.add_argument("-v", "--video-path", type=Path, required=True)
    parser.add_argument("--desired-fps", type=float, default=60.0)
    args = parser.parse_args()
    extract_frames(args.video_path, args.output_dir, args.desired_fps)


if __name__ == "__main__":
    main()
