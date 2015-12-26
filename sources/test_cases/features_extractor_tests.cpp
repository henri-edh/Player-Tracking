#include <opencv2/highgui/highgui.hpp>
#include "../../headers/test_cases/features_extractor_tests.h"
#include "../../headers/player_t.h"
#include "../../headers/features_t.h"
#include "../../headers/config.h"

namespace tmd{
    void FeaturesExtractorTest::setUp() {
        m_player = new player_t;
        m_player_image = cv::imread("./res/tests/player0.jpg");
        m_player->original_image = m_player_image;
        const int rows = m_player->original_image.rows;
        const int cols = m_player->original_image.cols;
        m_player->mask_image = cv::Mat(rows, cols, CV_8U);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                m_player->mask_image.at<uchar>(i, j) = 255;
            }
        }
        m_model_file = "./res/xmls/person.xml";
        m_hue_120_image = cv::imread("./res/tests/hue120.jpg");
        m_sat_05_image = cv::imread("./res/tests/sat50.jpg");
        m_val_05_image = cv::imread("./res/tests/val50.jpg");
    }

    void FeaturesExtractorTest::tearDown() {
        delete m_player;
    }

    void FeaturesExtractorTest::testExtractBodyPartsCorrectNumberOfParts() {
        tmd::DPMDetector d;
        d.extractBodyParts(m_player);
        CPPUNIT_ASSERT(m_player->features.body_parts.size() == 6);
    }

    void FeaturesExtractorTest::testHSVConversionIsCorrectForHue() {
        player_t* p = new player_t;
        p->features.torso = m_hue_120_image;
        tmd::FeaturesExtractor fe;
        fe.convertToHSV(p);
        const int cols = p->features.torso.cols;
        const int rows = p->features.torso.rows;
        for (int i = 0 ; i < rows ; i ++){
            for (int j = 0 ; j < cols ; j ++){
                cv::Vec3b color = p->features.torso.at<cv::Vec3b>(i, j);
                int hue = color[0];
                CPPUNIT_ASSERT(hue == 120 / 2.f);
            }
        }
        delete p;
    }

    void FeaturesExtractorTest::testHSVConversionIsCorrectForSaturation() {
        player_t* p = new player_t;
        p->features.torso = m_sat_05_image;
        tmd::FeaturesExtractor fe;
        fe.convertToHSV(p);
        const int cols = p->features.torso.cols;
        const int rows = p->features.torso.rows;
        for (int i = 0 ; i < rows ; i ++){
            for (int j = 0 ; j < cols ; j ++){
                cv::Vec3b color = p->features.torso.at<cv::Vec3b>(i, j);
                int sat = color[1];
                CPPUNIT_ASSERT(sat == ceil(0.5f*255));
            }
        }
        delete p;
    }

    void FeaturesExtractorTest::testHSVConversionIsCorrectForValue() {
        player_t* p = new player_t;
        p->features.torso = m_val_05_image;
        tmd::FeaturesExtractor fe;
        fe.convertToHSV(p);
        const int cols = p->features.torso.cols;
        const int rows = p->features.torso.rows;
        for (int i = 0 ; i < rows ; i ++){
            for (int j = 0 ; j < cols ; j ++){
                cv::Vec3b color = p->features.torso.at<cv::Vec3b>(i, j);
                int val = color[2];
                CPPUNIT_ASSERT(val == ceil(0.5f*255));
            }
        }
        delete p;
    }

    void FeaturesExtractorTest::testMaskIsUpdatedInRespectWithThresholds() {
        FeaturesExtractor fe;
        fe.extractFeatures(m_player);
        const int cols = m_player->features.torso_mask.cols;
        const int rows = m_player->features.torso_mask.rows;
        for (int i = 0 ; i < rows ; i ++) {
            for (int j = 0; j < cols; j++) {
                cv::Vec3b color = m_player->features.torso.at<cv::Vec3b>(i, j);
                bool withinTh = FeaturesExtractor::withinThresholds(color[0],
                                                        color[1], color[2]);
                if (withinTh){
                    CPPUNIT_ASSERT(m_player->features.torso_mask.at<uchar>(i, j)
                                   == 255);
                }
                else{
                    CPPUNIT_ASSERT(m_player->features.torso_mask.at<uchar>(i, j)
                                   == 0);
                }
            }
        }
    }

    void FeaturesExtractorTest::testHistogramsContainsSameNumberOfValues() {
        FeaturesExtractor fe;
        fe.extractFeatures(m_player);
        int mask_count = 0;
        const int cols = m_player->features.torso_mask.cols;
        const int rows = m_player->features.torso_mask.rows;
        for (int i = 0 ; i < rows ; i ++) {
            for (int j = 0; j < cols; j++) {
                if (m_player->features.torso_mask.at<uchar>(i, j) == 255){
                    mask_count ++;
                }
            }
        }

        int hist_count = 0;
        cv::Mat hist = m_player->features.torso_color_histogram;
        for (int i = 0 ; i < Config::feature_extractor_histogram_size ; i ++){
            hist_count += static_cast<int>(hist.at<float>(i));
        }

        CPPUNIT_ASSERT(mask_count == hist_count);
    }
}