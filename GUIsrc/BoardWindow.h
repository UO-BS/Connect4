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
    //Board Texture
    LPCWSTR m_boardPath;
    topDownGeometry::Rect m_boardSrcRect;

    //Piece Textures
    std::map<Player, std::tuple<LPCWSTR,LPCWSTR,topDownGeometry::Rect>> m_pieceTextures;
    std::vector<topDownGeometry::Rect> m_destinationRects; //These are in relation to the board texture, scaling down the board will also scale these

    //Column Selection Texture
    std::vector<topDownGeometry::Rect> m_columnRects; //These are in relation to the board texture, scaling down the board will also scale these
    LPCWSTR m_selectionPath;                    
    topDownGeometry::Rect m_selectionSrcRect; //These are in relation to the board texture, scaling down the board will also scale these

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
    bool setPieceDestinationRects(std::vector<topDownGeometry::Rect> newRects);
    //bool setPieceDestinationRects(std::vector<?> newRects); NEEDS TO AVOID USING MY GEOMETRY

    void setColumnSelectionTexture(LPCWSTR selectionPath, int srcHeight, int srcWidth);
    void setColumnSelectionTexture(LPCWSTR selectionPath, topDownGeometry::Rect srcRect);
    bool setColumnSelectionRects(std::vector<topDownGeometry::Rect> newRects);
    //bool setColumnSelectionRects(std::vector<?> newRects); NEEDS TO AVOID USING MY GEOMETRY

    void rescaleAllTextureRects(); //NOT DONE YET
};

#endif


/*
To Do:
    ->When drawing PieceInPosition, apply mask and use stretchBlt to scale to rectangle size
*/