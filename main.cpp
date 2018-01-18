#include "main.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {

    bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN();           // attempt KNN training

    if (!blnKNNTrainingSuccessful) {                                    // if KNN training was not successful
        // show error message
        std::cout << std::endl << std::endl << "error: error: KNN traning was not successful" << std::endl << std::endl;
        return(0);                                                      // and exit program
    }

    if (argc < 3 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        displayHelp();
        return(0);
    }

    if (strcmp(argv[1], "-p") == 0 || strcmp(argv[1], "--photo") == 0) {
        recognizePhotos(argc, argv);
        return(0);
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--video") == 0) {
        recognizeVideos(argc, argv);
        return(0);
    }

    return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawRedRectangleAroundPlate(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
    cv::Point2f p2fRectPoints[4];

    licPlate.rrLocationOfPlateInScene.points(p2fRectPoints);            // get 4 vertices of rotated rect

    for (int i = 0; i < 4; i++) {                                       // draw 4 red lines
        cv::line(imgOriginalScene, p2fRectPoints[i], p2fRectPoints[(i + 1) % 4], SCALAR_RED, 2);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void writeLicensePlateCharsOnImage(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
    cv::Point ptCenterOfTextArea;                   // this will be the center of the area the text will be written to
    cv::Point ptLowerLeftTextOrigin;                // this will be the bottom left of the area that the text will be written to

    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;                              // choose a plain jane font
    double dblFontScale = (double)licPlate.imgPlate.rows / 30.0;            // base font scale on height of plate area
    int intFontThickness = (int)std::round(dblFontScale * 1.5);             // base font thickness on font scale
    int intBaseline = 0;

    cv::Size textSize = cv::getTextSize(licPlate.strChars, intFontFace, dblFontScale, intFontThickness, &intBaseline);      // call getTextSize

    ptCenterOfTextArea.x = (int)licPlate.rrLocationOfPlateInScene.center.x;         // the horizontal location of the text area is the same as the plate

    if (licPlate.rrLocationOfPlateInScene.center.y < (imgOriginalScene.rows * 0.75)) {      // if the license plate is in the upper 3/4 of the image
        // write the chars in below the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) + (int)std::round((double)licPlate.imgPlate.rows * 1.2);
    }
    else {                                                                                // else if the license plate is in the lower 1/4 of the image
        // write the chars in above the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) - (int)std::round((double)licPlate.imgPlate.rows * 1.2);
    }

    ptLowerLeftTextOrigin.x = (int)(ptCenterOfTextArea.x - (textSize.width / 2));           // calculate the lower left origin of the text area
    ptLowerLeftTextOrigin.y = (int)(ptCenterOfTextArea.y + (textSize.height / 2));          // based on the text area center, width, and height

    // write the text on the image
    cv::putText(imgOriginalScene, licPlate.strChars, ptLowerLeftTextOrigin, intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);
}

void displayHelp() {
    printf("USAGE:\n");
    printf("\tpoo-proiect  [-p <photo_file_path>] [-v <video_file_path>] [-h]\n");
    printf("\n");
    printf("\n");
    printf("Where:\n");

    printf("\n");
    printf("\t-p <photo_file_path> OR --photo <photo_file_path>\n");
    printf("\t\tA list of file path(s) for the photo(s) that we're going to process.\n");

    printf("\n");
    printf("\t-v <photo_file_path> OR --video <video_file_path>\n");
    printf("\t\tFull file path for the video that we're going to process.\n");

    printf("\n");
    printf("\t-h OR --help\n");
    printf("\t\tDisplays usage information and exits.\n");
}

void recognizeFrame(cv::Mat imgOriginalScene, std::string &name) {
    if (imgOriginalScene.empty()) {                             // if unable to open image
        std::cout << "error: image not read from file\n\n";     // show error message on command line
        cv::waitKey(0);                                         // may have to modify this line if not using Windows
        return;                                              // and exit program
    }

    std::vector<PossiblePlate> vectorOfPossiblePlates = detectPlatesInScene(imgOriginalScene);          // detect plates

    vectorOfPossiblePlates = detectCharsInPlates(vectorOfPossiblePlates);                               // detect chars in plates

    cv::imshow(name +"imgOriginalScene", imgOriginalScene);           // show scene image

    if (vectorOfPossiblePlates.empty()) {                                               // if no plates were found
        std::cout << std::endl << "no license plates were detected" << std::endl;       // inform user no plates were found
    }
    else {                                                                            // else
        // if we get in here vector of possible plates has at leat one plate

        // sort the vector of possible plates in DESCENDING order (most number of chars to least number of chars)
        std::sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);

        // suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
        PossiblePlate licPlate = vectorOfPossiblePlates.front();

        cv::imshow(name + "_imgPlate", licPlate.imgPlate);            // show crop of plate and threshold of plate
        cv::imshow(name + "_imgThresh", licPlate.imgThresh);

        if (licPlate.strChars.length() == 0) {                                                      // if no chars were found in the plate
            std::cout << std::endl << "no characters were detected" << std::endl << std::endl;      // show message
            return;                                                                              // and exit program
        }

        drawRedRectangleAroundPlate(imgOriginalScene, licPlate);                // draw red rectangle around plate

        std::cout << std::endl << "license plate read from image = " << licPlate.strChars << std::endl;     // write license plate text to std out
        std::cout << std::endl << "-----------------------------------------" << std::endl;

        writeLicensePlateCharsOnImage(imgOriginalScene, licPlate);              // write license plate text on the image

        cv::imshow(name + "_imgOriginalScene", imgOriginalScene);                       // re-show scene image

        //cv::imwrite("imgOriginalScene.png", imgOriginalScene);                  // write image out to file

        cv::waitKey(0);
    }
}

void recognizePhotos(int argc, char **argv) {
    cv::Mat imgOriginalScene;           // input image

    std::string currentFrameName;
    for (int i = 2; i < argc; ++i) {
        currentFrameName = argv[i];
        imgOriginalScene = cv::imread(currentFrameName);         // open image

        currentFrameName = currentFrameName.substr(currentFrameName.rfind('/') + 1); //parse file name

        recognizeFrame(imgOriginalScene, currentFrameName);
    }
}

void recognizeVideos(int argc, char **argv) {
    printf("not yet implemented");
}
