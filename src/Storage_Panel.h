//
// Created by evan on 2026-03-08.
//

#ifndef GROUP55_STORAGE_PANEL_H
#define GROUP55_STORAGE_PANEL_H

#include <wx/wx.h>
#include <list>
#include <wx/mediactrl.h>

class UI;
class SecuritySystem;

/**
 * @brief Panel that displays a scrollable gallery of stored images.
 *
 * Fetches images from the SecuritySystem, renders them as thumbnails
 * in a responsive grid, and provides options to view or delete each image.
 */
class Storage_Panel : public wxPanel {
public:
    /**
    * @brief Constructs the Storage_Panel.
    * @param parent The parent wxWindow.
    * @param system Pointer to the SecuritySystem for accessing stored images.
    * @param ui Pointer to the main UI frame for panel navigation.
    */
    Storage_Panel(wxWindow* parent, SecuritySystem* system, UI* ui);

    ~Storage_Panel();

    /**
     * @brief Fetches and displays all images from storage in the gallery grid.
     *
     * Clears any existing thumbnails before reloading. Displays an error
     * label if no images are found.
     */
    void loadImages();

private:
    SecuritySystem* m_system;      ///< Pointer to the security system
    UI* m_ui;                      ///< Pointer to the main UI frame
    wxStaticText* errorLabel;      ///< Label shown when no images are available
    wxFlexGridSizer* gridSizer;    ///< Grid sizer managing thumbnail layout
    wxScrolledWindow* galleryPanel;///< Scrollable container for the image grid
    wxBoxSizer* outerSizer;        ///< Outer vertical sizer for the panel

    /**
     * @brief Loads and scales an image from disk into a thumbnail bitmap.
     * @param filepath The path to the image file.
     * @return A scaled wxBitmap, or wxNullBitmap if loading failed.
     */
    wxBitmap loadThumbnail(const wxString& filepath);

    /**
     * @brief Handles gallery resize events to adjust the number of grid columns.
     * @param event The wxSizeEvent triggered on panel resize.
     */
    void OnGalleryResize(wxSizeEvent& event);

    /**
     * @brief Opens a fullscreen image viewer for the selected image.
     * @param path The file path of the image to display.
     */
    void OpenFullImage(const wxString& path);

    /**
     * @brief Handles the Back Home button press, navigating to the Home panel.
     * @param event The wxCommandEvent triggered by the back home button.
     */
    void onBackHome(wxCommandEvent& event);

    void openVideo(const wxString& path);
    void isVideoFile(const wxString& path);

};


#endif //GROUP55_STORAGE_PANEL_H