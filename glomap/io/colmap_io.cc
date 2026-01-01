#include "glomap/io/colmap_io.h"
#include "glomap/io/recording.h"

#include <colmap/util/file.h>
#include <colmap/util/misc.h>

namespace glomap {

void WriteGlomapReconstruction(
    const std::string& reconstruction_path,
    const std::unordered_map<rig_t, Rig>& rigs,
    const std::unordered_map<camera_t, Camera>& cameras,
    const std::unordered_map<frame_t, Frame>& frames,
    const std::unordered_map<image_t, Image>& images,
    const std::unordered_map<track_t, Track>& tracks,
    const std::string output_format,
    const std::string image_path) {
  // Check whether reconstruction pruning is applied.
  // If so, export seperate reconstruction
  int largest_component_num = -1;
  for (const auto& [frame_id, frame] : frames) {
    if (frame.cluster_id > largest_component_num)
      largest_component_num = frame.cluster_id;
  }
  // If it is not seperated into several clusters, then output them as whole
  if (largest_component_num == -1) {
    colmap::Reconstruction reconstruction;
    ConvertGlomapToColmap(
        rigs, cameras, frames, images, tracks, reconstruction);
    // Read in colors
    if (image_path != "") {
      LOG(INFO) << "Extracting colors ...";
      reconstruction.ExtractColorsForAllImages(image_path);
    }

    // Convert back to GLOMAP so that we can log the reconstruction with color.
    std::unordered_map<rig_t, Rig> rigs_copy;
    std::unordered_map<camera_t, Camera> cameras_copy;
    std::unordered_map<frame_t, Frame> frames_copy;
    std::unordered_map<image_t, Image> images_copy;
    std::unordered_map<track_t, Track> tracks_copy;
    ConvertColmapToGlomap(reconstruction, rigs_copy, cameras_copy, frames_copy, images_copy, tracks_copy);
    rr_rec.set_time_sequence("step", algorithm_step++);
    rr_rec.log("status", rerun::TextLog("Converted to Colmap and extracted colors"));
    log_reconstruction(rr_rec, cameras_copy, images_copy, tracks_copy);
    
    colmap::CreateDirIfNotExists(reconstruction_path + "/0", true);
    if (output_format == "txt") {
      reconstruction.WriteText(reconstruction_path + "/0");
    } else if (output_format == "bin") {
      reconstruction.WriteBinary(reconstruction_path + "/0");
    } else {
      LOG(ERROR) << "Unsupported output type";
    }
  } else {
    for (int comp = 0; comp <= largest_component_num; comp++) {
      std::cout << "\r Exporting reconstruction " << comp + 1 << " / "
                << largest_component_num + 1 << std::flush;
      colmap::Reconstruction reconstruction;
      ConvertGlomapToColmap(
          rigs, cameras, frames, images, tracks, reconstruction, comp);
      // Read in colors
      if (image_path != "") {
        reconstruction.ExtractColorsForAllImages(image_path);
      }
      
      colmap::CreateDirIfNotExists(
          reconstruction_path + "/" + std::to_string(comp), true);
      if (output_format == "txt") {
        reconstruction.WriteText(reconstruction_path + "/" +
                                 std::to_string(comp));
      } else if (output_format == "bin") {
        reconstruction.WriteBinary(reconstruction_path + "/" +
                                   std::to_string(comp));
      } else {
        LOG(ERROR) << "Unsupported output type";
      }
    }
    std::cout << std::endl;
  }
}

void WriteColmapReconstruction(const std::string& reconstruction_path,
                               const colmap::Reconstruction& reconstruction,
                               const std::string output_format) {
  colmap::CreateDirIfNotExists(reconstruction_path, true);
  if (output_format == "txt") {
    reconstruction.WriteText(reconstruction_path);
  } else if (output_format == "bin") {
    reconstruction.WriteBinary(reconstruction_path);
  } else {
    LOG(ERROR) << "Unsupported output type";
  }
}

}  // namespace glomap
