#ifndef BACHELOR_PROJECT_DPM_PLAYER_EXTRACTOR_H
#define BACHELOR_PROJECT_DPM_PLAYER_EXTRACTOR_H

#include <opencv2/objdetect/objdetect.hpp>
#include "../player_extractor.h"
#include <iostream>
#include "../../misc/debug.h"
#include "../../misc/config.h"
#include "../../features_extraction/dpm.h"


namespace tmd{

    /**
     * Player extractor using DPM to detect and crop players from the frame.
     */
    class DPMPlayerExtractor : public tmd::PlayerExtractor{
    public :
        /**
         * Constructor of the DPMPlayerExtractor
         */
        DPMPlayerExtractor();

        ~DPMPlayerExtractor();

        /**
         * Extract a player from a given frame. Returns a vector containing
         * the detected players.
         */
        virtual std::vector<player_t*> extract_player_from_frame(frame_t*
        frame);

        /**
         * Setters for the 2 thresholds.
         */
        void set_overlapping_threshold(float th);
        void set_score_threshold(float th);

        /**
         * Getters for the 2 thresholds.
         */
        float get_overlapping_threshold();
        float get_score_threshold();

    private:
        void recreate_detector();
        tmd::DPM* m_detector;
    };
}

#endif //BACHELOR_PROJECT_DPM_PLAYER_EXTRACTOR_H
