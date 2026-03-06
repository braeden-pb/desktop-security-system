//
// Created by Braeden Patierno-Barker on 3/6/2026.
//

Class Storage {
    public:
    Storage();
    ~Storage();
    std::string saveImage(Image image) {
        return image->getPath();
    }
    bool deleteImage(Image imageID) {
        return false;
    }
    list<Image> listImage() {
        return list<Image>();
    }
    bool hasTimeStamp(String imageID) {
        return false;
    }
}