#ifndef BOARD_WINDOW_H
#define BOARD_WINDOW_H

#include "BaseWindow.h"
#include "TopDownGeometry.h"
#include "ConnectGame.h"
#include <map>
#include <tuple>
#include <vector>

class BoardWindow : public BaseWindow<BoardWindow>
{
private:
    //Background Board Texture
    LPCWSTR m_boardPath;
    //Rect to hold the board image dimensions
    topDownGeometry::Rect m_boardSrcRect;
    //Rect to hold the resized board dimensions
    topDownGeometry::Rect m_rescaledBoardRect;

    //Textures for when a Piece is inside the board, contained in a map because each player has a unique piece
    std::map<Player, std::tuple<LPCWSTR,LPCWSTR,topDownGeometry::Rect>> m_pieceTextures;
    //Vector of Rects that hold a resizing of the image in order to fit on the board correctly (customizable)
    std::vector<topDownGeometry::Rect> m_pieceRects;
    //Vector of Rects that hold the final dimensions for displaying the image
    std::vector<topDownGeometry::Rect> m_rescaledPieceRects;

    //Column Selection Texture (goes above the selected column)
    LPCWSTR m_selectionPath;
    //Rect to hold the Column Selection image dimensions
    topDownGeometry::Rect m_selectionSrcRect;
    //Vector of Rects that hold the "hover zone" for each column to be selected (customizable)
    std::vector<topDownGeometry::Rect> m_columnRects;
    //Vector of Rects that hold the final dimensions for the "hover zone" of each column
    std::vector<topDownGeometry::Rect> m_rescaledColumnRects;
    

    //CurrentGame
    const ConnectGame& m_currentGame;
public:
    PCWSTR  className() const;
    LRESULT handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    BoardWindow()=delete;
    BoardWindow(const ConnectGame& newGame, bool useDefaults=true);

    void useDefaultTextures();

    void setBoardTexture(LPCWSTR boardPath, int srcHeight, int srcWidth);
    void setBoardTexture(LPCWSTR boardPath, topDownGeometry::Rect srcRect);

    void addPieceTexture(Player player, LPCWSTR colorPath, LPCWSTR maskPath, int srcHeight, int srcWidth);
    //void addPieceTexture(Player player, LPCWSTR colorPath, LPCWSTR maskPath, topDownGeometry::Rect srcRect);
    bool setPieceRects(std::vector<topDownGeometry::Rect> newRects);
    //bool setPieceRects(std::vector<?> newRects); NEEDS TO AVOID USING MY GEOMETRY

    void setColumnSelectionTexture(LPCWSTR selectionPath, int srcHeight, int srcWidth);
    void setColumnSelectionTexture(LPCWSTR selectionPath, topDownGeometry::Rect srcRect);
    bool setColumnSelectionRects(std::vector<topDownGeometry::Rect> newRects);
    //bool setColumnSelectionRects(std::vector<?> newRects); NEEDS TO AVOID USING MY GEOMETRY

    void resizeAllRects(int windowWidth, int windowHeight);
};

#endif


/*
To Do:
    ->When drawing PieceInPosition, apply mask and use stretchBlt to scale to rectangle size
*/