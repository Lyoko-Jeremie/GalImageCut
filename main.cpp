#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

#include <boost/assert.hpp>

namespace boost {
    void assertion_failed(char const *expr, char const *function, char const *file, long line) {
        BOOST_LOG_TRIVIAL(error)
            << "assertion_failed : [" << expr << "]"
            << " on function [" << function << "]"
            << " on file [" << file << "]"
            << " at line [" << line << "]";
        std::abort();
    }

    void assertion_failed_msg(char const *expr, char const *msg, char const *function, char const *file, long line) {
        BOOST_LOG_TRIVIAL(error)
            << "assertion_failed_msg : [" << expr << "]"
            << " msg [" << msg << "]"
            << " on function [" << function << "]"
            << " on file [" << file << "]"
            << " at line [" << line << "]";
        std::abort();
    }
}

int main(int argc, char *argv[]) {
    BOOST_LOG_TRIVIAL(info) << "Hello, World!";

    BOOST_LOG_TRIVIAL(info) << "argc " << argc;
    std::vector<std::string> args{};
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    for (auto &a: args) {
        BOOST_LOG_TRIVIAL(info) << "args " << a;
    }

    if (args.size() < 2) {
        BOOST_LOG_TRIVIAL(error) << "(argc < 2)";
        return 0;
    }

    auto filePath = boost::filesystem::path(args.at(1));
    bool fileCheck = boost::filesystem::is_regular_file(filePath);
    BOOST_LOG_TRIVIAL(info) << "fileCheck " << fileCheck;
    BOOST_LOG_TRIVIAL(info) << "filename " << filePath.filename();

    auto img = cv::imread(filePath.string());
    if (img.empty()) {
        BOOST_LOG_TRIVIAL(error) << "(img.empty())";
        return 0;
    }

//    cv::namedWindow(
//            "img",
//            cv::WindowFlags::WINDOW_GUI_NORMAL
//    );
//    cv::imshow("img", img);
//    while (cv::waitKey() != 27);


    BOOST_LOG_TRIVIAL(info) << img.size;


    auto roi = img(
            cv::Rect{
                    cv::Point2i{1, 37},
                    cv::Size2i{img.size[1] - 2, img.size[0] - 37 - 1}
            }
    );

//    cv::namedWindow(
//            "roi",
//            cv::WindowFlags::WINDOW_GUI_NORMAL
//    );
//    cv::imshow("roi", roi);
//    while (cv::waitKey() != 27);


//    BOOST_LOG_TRIVIAL(info) << "filename new "
//                            << filePath.filename().stem().string()
//                            << "-cut-"
//                            << filePath.filename().extension().string();
    std::string newName = filePath.filename().stem().string() + "-cut-.png";
    std::string newPath = (filePath.parent_path() / newName).string();
    BOOST_LOG_TRIVIAL(info) << "filename new " << newName;
    BOOST_LOG_TRIVIAL(info) << "path new " << newPath;

    cv::imwrite(newPath, roi);


    return 0;
}
