#ifndef UNICODE
#define UNICODE
#endif 

#include "BoardWindow.h"
#include <iostream>

BoardWindow::BoardWindow(ConnectGame& newGame, bool useDefaults) : BaseWindow{}, m_currentGame{newGame}
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

    case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam); //GET_X_LPARAM not needed here
            int y = HIWORD(lParam); //GET_Y_LPARAM not needed here
            topDownGeometry::Point location{x,y};

            RECT topBar;
            topBar.top=0; topBar.bottom=m_rescaledBoardRect.getTopLeft().getY();
            topBar.left=0; topBar.right=m_rescaledBoardRect.getTopRight().getX();
            
            if (m_hoveredColumn==-1) { //If the mouse was not previously hovering over a column
                for (int i=0;i<m_columnRects.size();i++) {
                    if (m_rescaledColumnRects[i].pointInRect(location)) { //The mouse has entered a new column
                        m_hoveredColumn = i;

                        InvalidateRect(m_hwnd,&topBar,TRUE);
                    }
                }
            } else {    //If the mouse was previously hovering over a column
                if (!m_rescaledColumnRects[m_hoveredColumn].pointInRect(location)) {    //Has the mouse left the hoveredcolumn?
                    for (int i=0;i<m_columnRects.size();i++) {
                        if (m_rescaledColumnRects[i].pointInRect(location)) {   //The mouse has entered a new column
                            m_hoveredColumn = i;
                            
                            InvalidateRect(m_hwnd,&topBar,TRUE);

                        } else {    //The mouse has not entered a new column
                            m_hoveredColumn = -1;

                            InvalidateRect(m_hwnd,&topBar,TRUE);
                        }
                    }
                }
            }
        }
        return 0;

    case WM_LBUTTONDOWN:
        {
            if (m_hoveredColumn!=-1) {  //Making sure the user clicked on a column
                if (m_currentGame.dropPiece(m_hoveredColumn+1, Piece{m_currentGame.getCurrentTurn()})){
                    //Piece successfully dropped, redraw position
                    for (int i=0;i<m_currentGame.getRowNumber();i++) {
                        if (m_currentGame.getBoard()[i][m_hoveredColumn]) {
                            RECT pieceRect;
                            pieceRect.top   =m_rescaledPieceRects[i*m_currentGame.getColumnNumber()+m_hoveredColumn].getTopLeft().getY(); 
                            pieceRect.bottom=m_rescaledPieceRects[i*m_currentGame.getColumnNumber()+m_hoveredColumn].getBottomRight().getY();
                            pieceRect.left  =m_rescaledPieceRects[i*m_currentGame.getColumnNumber()+m_hoveredColumn].getTopLeft().getX(); 
                            pieceRect.right =m_rescaledPieceRects[i*m_currentGame.getColumnNumber()+m_hoveredColumn].getBottomRight().getX();

                            InvalidateRect(m_hwnd,&pieceRect,TRUE);
                            break;
                        }
                    }
                }
                //Inform parent that a selection was made
                SendMessage(GetParent(m_hwnd), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(m_hwnd), WM_LBUTTONDOWN), (LPARAM)m_hwnd);
            }
        }
        return 0;


    case WM_SIZE:
        resizeAllRects(LOWORD(lParam),HIWORD(lParam),true);
        RedrawWindow(m_hwnd,NULL,NULL, RDW_INVALIDATE);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(m_hwnd, &ps);

            if (ps.fErase) { //Background needs to be erased
                HBRUSH brush = CreateSolidBrush(0x00FFFFFF);
                FillRect(hdc,&ps.rcPaint,brush);
                DeleteObject(brush);
            }
            
            paintBoard(hdc);
            paintPieces(hdc);
            paintSelection(hdc);

            EndPaint(m_hwnd, &ps);
        }
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

void BoardWindow::paintBoard(const HDC& hdc)
{
    HBITMAP bmp = (HBITMAP) LoadImage(NULL,m_boardPath,IMAGE_BITMAP,m_boardSrcRect.getWidth(),m_boardSrcRect.getHeight(),LR_LOADFROMFILE);
    if (!bmp) {
        std::cout << "Error Loading .bmp file: " << GetLastError() << "\n";
        std::cout << "Image path: " << m_boardPath << "\n";
    }
    HDC tempDC = CreateCompatibleDC(hdc);
    HGDIOBJ replacedOBJ = SelectObject(tempDC, bmp);
    StretchBlt(hdc,m_rescaledBoardRect.getTopLeft().getX(),m_rescaledBoardRect.getTopLeft().getY(),
                m_rescaledBoardRect.getWidth(),m_rescaledBoardRect.getHeight(),tempDC,
                0,0,m_boardSrcRect.getWidth(),m_boardSrcRect.getHeight(),SRCCOPY);
    SelectObject(tempDC, replacedOBJ);
    DeleteObject(bmp);
    DeleteDC(tempDC);
}
void BoardWindow::paintPieces(const HDC& hdc)
{
    HDC tempDC = CreateCompatibleDC(hdc);

    for (int playerNum=0;playerNum<m_pieceTextures.size();playerNum++) {
        std::tuple<LPCWSTR,LPCWSTR,topDownGeometry::Rect> playerData = m_pieceTextures.find(m_currentGame.getPlayerList()[playerNum])->second;
        HBITMAP bmp = (HBITMAP) LoadImage(NULL,std::get<0>(playerData),IMAGE_BITMAP,std::get<2>(playerData).getWidth(),std::get<2>(playerData).getHeight(),LR_LOADFROMFILE);
        HBITMAP bmpMask = (HBITMAP) LoadImage(NULL,std::get<1>(playerData),IMAGE_BITMAP,std::get<2>(playerData).getWidth(),std::get<2>(playerData).getHeight(),LR_LOADFROMFILE);
        if (!bmp || !bmpMask) {
            std::cout << "Error Loading .bmp file: " << GetLastError() << "\n";
            std::cout << "Piece Image failed to load";
        }
        HGDIOBJ replacedOBJ = SelectObject(tempDC, bmpMask);

        for (int row=0;row<m_currentGame.getRowNumber();row++) {
            for (int column=0;column<m_currentGame.getColumnNumber();column++) {
                if (m_currentGame.getBoard()[row][column]) {
                    if (m_currentGame.getBoard()[row][column].getOwner() == m_currentGame.getPlayerList()[playerNum]) {
                    
                    topDownGeometry::Rect destination = m_rescaledPieceRects[row*m_currentGame.getColumnNumber()+column];
                    
                    StretchBlt(hdc,destination.getTopLeft().getX(),destination.getTopLeft().getY(),
                                destination.getWidth(),destination.getHeight(),tempDC,
                                0,0,std::get<2>(playerData).getWidth(),std::get<2>(playerData).getHeight(),SRCAND);
                    SelectObject(tempDC, bmp);
                    StretchBlt(hdc,destination.getTopLeft().getX(),destination.getTopLeft().getY(),
                                destination.getWidth(),destination.getHeight(),tempDC,
                                0,0,std::get<2>(playerData).getWidth(),std::get<2>(playerData).getHeight(),SRCPAINT);
                    SelectObject(tempDC, bmpMask);
                    }
                }
            }
        }

        SelectObject(tempDC, replacedOBJ);
        DeleteObject(bmp);
        DeleteObject(bmpMask);
    }
    
    DeleteDC(tempDC);
}

void BoardWindow::paintSelection(const HDC& hdc)
{
    if (m_hoveredColumn==-1) {
        return;
    }

    HBITMAP bmp = (HBITMAP) LoadImage(NULL,m_selectionPath,IMAGE_BITMAP,m_selectionSrcRect.getWidth(),m_selectionSrcRect.getHeight(),LR_LOADFROMFILE);
    if (!bmp) {
        std::cout << "Error Loading .bmp file: " << GetLastError() << "\n";
        std::cout << "Image path: " << m_selectionPath << "\n";
    }
    HDC tempDC = CreateCompatibleDC(hdc);
    HGDIOBJ replacedOBJ = SelectObject(tempDC, bmp);
    StretchBlt(hdc,m_rescaledColumnRects[m_hoveredColumn].getTopLeft().getX(),0,
                m_rescaledColumnRects[m_hoveredColumn].getWidth(),m_rescaledBoardRect.getTopLeft().getY(),tempDC,
                0,0,m_selectionSrcRect.getWidth(),m_selectionSrcRect.getHeight(),SRCCOPY);
    SelectObject(tempDC, replacedOBJ);
    DeleteObject(bmp);
    DeleteDC(tempDC);
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
    for (int row=0;row<m_currentGame.getRowNumber();row++) {
        for (int column=0;column<m_currentGame.getColumnNumber();column++) {
            newRects.push_back(topDownGeometry::Rect{topDownGeometry::Point{50+column*100,50+row*100},100,100});
        }
    }
    if (!setPieceRects(newRects)) {
        std::cout << "Set Piece Failed";
    }

    setColumnSelectionTexture(L"../resources/defaultSelection.bmp",topDownGeometry::Rect{topDownGeometry::Point{0,0},100,100});
    std::vector<topDownGeometry::Rect> newColumns;
    LPRECT rcClient; //NEED TO DELETE?
    newColumns.reserve(m_currentGame.getColumnNumber());
    GetClientRect(m_hwnd,rcClient);
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        newColumns.push_back(topDownGeometry::Rect{topDownGeometry::Point{50+(i*100),0},100,m_boardSrcRect.getHeight()});
    }
    if (!setColumnSelectionRects(newColumns)) {
        std::cout << "Set ColumnSelection Failed";
    }
}

void BoardWindow::resizeAllRects(int windowWidth, int windowHeight, bool maintainRatio)
{
    int topLimit = windowHeight/8 +1; //+1 to make sure the textures dont overlap

    topDownGeometry::Rect newBoardRect;
    if (maintainRatio) {
        double srcRatio = static_cast<double>(m_boardSrcRect.getWidth()) / static_cast<double>(m_boardSrcRect.getHeight());
        double newRatio = static_cast<double>(windowWidth) / static_cast<double>(windowHeight-topLimit);
        if (srcRatio > newRatio) {  //Width is the limiting dimension
            double widthRatio = static_cast<double>(m_boardSrcRect.getWidth()) / static_cast<double>(windowWidth);
            newBoardRect = topDownGeometry::Rect{topDownGeometry::Point{0,topLimit},
                                                    static_cast<int>(m_boardSrcRect.getWidth()/widthRatio),
                                                    static_cast<int>(m_boardSrcRect.getHeight()/widthRatio)};
        } else {    //Height is the limiting dimension
            double heightRatio = static_cast<double>(m_boardSrcRect.getHeight()) / static_cast<double>(windowHeight-topLimit);
            newBoardRect = topDownGeometry::Rect{topDownGeometry::Point{0,topLimit},
                                                    static_cast<int>(m_boardSrcRect.getWidth()/heightRatio),
                                                    static_cast<int>(m_boardSrcRect.getHeight()/heightRatio)};
        }
    } else {
        newBoardRect = topDownGeometry::Rect{topDownGeometry::Point{0,topLimit},windowWidth,windowHeight-topLimit};
    }
    
    //Resize m_rescaledBoardRect 
    m_rescaledBoardRect = m_boardSrcRect.transformRect(m_boardSrcRect, newBoardRect);
    
    //Resize m_rescaledPieceRects based on the resized board
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        for (int j=0;j<m_currentGame.getRowNumber();j++) {
            m_rescaledPieceRects[i*m_currentGame.getRowNumber()+j] = m_pieceRects[i*m_currentGame.getRowNumber()+j].transformRect(m_boardSrcRect, newBoardRect);
        }
    }

    //Resize m_rescaledColumnRects based on the resized board
    for (int i=0;i<m_currentGame.getColumnNumber();i++) {
        m_rescaledColumnRects[i] = m_columnRects[i].transformRect(m_boardSrcRect, newBoardRect);
    }

}
