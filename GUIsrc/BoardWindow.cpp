#ifndef UNICODE
#define UNICODE
#endif 

#include "BoardWindow.h"
#include <iostream>

BoardWindow::BoardWindow(const ConnectGame& newGame, bool useDefaults) : BaseWindow{}, m_currentGame{newGame}
{
    if (useDefaults) {
        useDefaultTextures();
    }
}

PCWSTR  BoardWindow::className() const { return L"BoardWindow"; }
LRESULT BoardWindow::handleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
        LRESULT lRes = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
        
        return lRes;
        }

    case WM_LBUTTONDOWN:
        if (GetParent(m_hwnd) != NULL) {
            SendMessage(GetParent(m_hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(m_hwnd), WM_LBUTTONDOWN), (LPARAM)m_hwnd);
        }
        return 0;

    case WM_SIZE:
        resizeAllRects(LOWORD(lParam),HIWORD(lParam));
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);
            
            HBITMAP bmp = (HBITMAP) LoadImage(NULL,m_boardPath,IMAGE_BITMAP,m_boardSrcRect.getWidth(),m_boardSrcRect.getHeight(),LR_LOADFROMFILE);
            if (!bmp) {
                std::cout << "Error Loading .bmp file: " << GetLastError() << "\n";
                std::cout << "Image path: " << m_boardPath << "\n";
            }
            HDC tempDC = CreateCompatibleDC(hdc);
            HGDIOBJ replacedOBJ = SelectObject(tempDC, bmp);
            BitBlt(hdc,0,100,m_boardSrcRect.getWidth(),m_boardSrcRect.getHeight(),tempDC,0,0,SRCCOPY);
            //StretchBlt(hdc,0,0,100,75,tempDC,0,0,m_srcWidth,m_srcHeight,SRCCOPY);
            //Draw pieces and selection
            SelectObject(tempDC, replacedOBJ);
            DeleteObject(bmp);
            DeleteDC(tempDC);

            EndPaint(m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

void BoardWindow::setBoardTexture(LPCWSTR boardPath, int srcHeight, int srcWidth)
{
    m_boardSrcRect = topDownGeometry::Rect{topDownGeometry::Point{0,0}, srcWidth, srcHeight};
    m_rescaledBoardRect = m_boardSrcRect;
    m_boardPath = boardPath;
}

void BoardWindow::setBoardTexture(LPCWSTR boardPath, topDownGeometry::Rect srcRect)
{
   m_boardSrcRect = srcRect;
   m_rescaledBoardRect = m_boardSrcRect;
   m_boardPath = boardPath;
}

void BoardWindow::addPieceTexture(Player player, LPCWSTR colorPath, LPCWSTR maskPath, int srcHeight, int srcWidth)
{
    //NEED TO CHECK FOR DUPLICATE, .insert returns std::pair<iterator,bool> where bool is if the insertion was success
    topDownGeometry::Rect tempRect{topDownGeometry::Point{0,0}, srcWidth, srcHeight};
    std::tuple<LPCWSTR,LPCWSTR,topDownGeometry::Rect> textureData;
    textureData = std::make_tuple(colorPath, maskPath, tempRect);
    m_pieceTextures.insert({player,textureData});
}

bool BoardWindow::setPieceRects(std::vector<topDownGeometry::Rect> newRects)
{
    //This does not check if they overlap each other
    if (newRects.size() != m_currentGame.getColumnNumber()*m_currentGame.getRowNumber()) {
        return false;
    }
    for (size_t i=0;i<newRects.size();i++) {
        if (!m_boardSrcRect.rectInRect(newRects[i])) {
            return false;
        }
    }
    m_pieceRects = newRects;
    m_rescaledPieceRects = m_pieceRects;
    return true;
}

void BoardWindow::setColumnSelectionTexture(LPCWSTR selectionPath, int srcHeight, int srcWidth)
{
    m_selectionSrcRect = topDownGeometry::Rect{topDownGeometry::Point{0,0}, srcWidth, srcHeight};
    m_selectionPath = selectionPath;
}
void BoardWindow::setColumnSelectionTexture(LPCWSTR selectionPath, topDownGeometry::Rect srcRect)
{
    m_selectionSrcRect = srcRect;
    m_selectionPath = selectionPath;
}
bool BoardWindow::setColumnSelectionRects(std::vector<topDownGeometry::Rect> newRects)
{
    //This does not check if they overlap each other
    if (newRects.size() != m_currentGame.getColumnNumber()) {
        return false;
    }
    for (size_t i=0;i<newRects.size();i++) {
        if (!m_boardSrcRect.rectInRect(newRects[i])) {
            return false;
        }
    }
    m_columnRects = newRects;
    m_rescaledColumnRects = m_columnRects;
    return true;
}

void BoardWindow::useDefaultTextures()
{
    setBoardTexture(L"../resources/defaultBoard.bmp",700,800);
    
    addPieceTexture(m_currentGame.getPlayerList()[0],L"../resources/defaultPiece1.bmp",L"../resources/defaultPieceMask.bmp",100,100);
    addPieceTexture(m_currentGame.getPlayerList()[1],L"../resources/defaultPiece2.bmp",L"../resources/defaultPieceMask.bmp",100,100);
    
    std::vector<topDownGeometry::Rect> newRects;
    newRects.reserve(m_currentGame.getRowNumber()*m_currentGame.getColumnNumber());
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        for (int j=0;j<m_currentGame.getRowNumber();j++) {
            newRects.push_back(topDownGeometry::Rect{topDownGeometry::Point{j*100,i*100},100,100});
        }
    }
    setPieceRects(newRects);

    setColumnSelectionTexture(L"../resources/defaultSelection.bmp",topDownGeometry::Rect{topDownGeometry::Point{0,0},100,100});
    std::vector<topDownGeometry::Rect> newColumns;
    LPRECT rcClient; //NEED TO DELETE?
    newColumns.reserve(m_currentGame.getColumnNumber());
    GetClientRect(m_hwnd,rcClient);
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        newColumns.push_back(topDownGeometry::Rect{topDownGeometry::Point{i*m_boardSrcRect.getWidth()/m_currentGame.getColumnNumber(),0},m_boardSrcRect.getWidth()/m_currentGame.getColumnNumber(),m_boardSrcRect.getHeight()});
    }
    setColumnSelectionRects(newColumns);
}

void BoardWindow::resizeAllRects(int windowWidth, int windowHeight)
{
    int topLimit = windowHeight/8 +1; //+1 to make sure the textures dont overlap

    //Resize m_rescaledBoardRect 
    topDownGeometry::Rect newBoardRect{topDownGeometry::Point{0,topLimit},windowWidth,windowHeight-topLimit};
    m_rescaledBoardRect.transformRect(m_boardSrcRect, newBoardRect);
    
    //Resize m_rescaledPieceRects based on the resized board
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        for (int j=0;j<m_currentGame.getRowNumber();j++) {
            m_rescaledPieceRects[i*m_currentGame.getRowNumber()+j].transformRect(m_boardSrcRect, newBoardRect);
        }
    }

    //Resize m_rescaledColumnRects based on the resized board
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        m_rescaledColumnRects[i].transformRect(m_boardSrcRect, newBoardRect);
    }

}
