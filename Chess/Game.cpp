#include "pch.h"
#include "Game.h"
#include "Chessboard.h"
#include "PromotionDialog.h"
#include "ChessDlg.h"
#include "resource.h"
#include <cmath>
#include <algorithm>
#include <vector>

// ----------------- Game -----------------

Game::Game(CChessDlg* parent) {
    parentDlg = parent;
}

void Game::Draw(HDC dc, RECT client_rect) {
    chessboard->DrawBoard(dc, client_rect);
}

void Game::switchTurn() {
    currentPlayer = (currentPlayer == white.get()) ? black.get() : white.get();
}

// ----------------- Castling Helper -----------------

bool Game::canCastle(const Square& kingSquare, const Square& rookSquare, const std::vector<std::vector<Square>>& board) {
    Piece kingPiece = kingSquare.GetPiece();
    Piece rookPiece = rookSquare.GetPiece();
    PieceColor color = kingPiece.GetPieceColor();

    if (!kingPiece.IsKing() || !rookPiece.IsRook()) return false;

    // cannot have moved before
    if (kingSquare.GetPiece().GetHasMoved() || rookSquare.GetPiece().GetHasMoved()) return false;

    int kingX, kingY, rookX, rookY;
    if (!findSquarePosition(const_cast<Square&>(kingSquare), board, kingX, kingY) ||
        !findSquarePosition(const_cast<Square&>(rookSquare), board, rookX, rookY)) return false;

    int dir = (rookX > kingX) ? 1 : -1;

    // path between king and rook must be clear (excluding rook square)
    for (int x = kingX + dir; x != rookX; x += dir) {
        if (board[kingY][x].GetPiece().GetPieceType() != PieceType::None) return false;
    }

    // king cannot be in check, or pass through a square under attack, or land in check
    for (int step = 0; step <= 2; ++step) {
        int testX = kingX + step * dir;
        if (testX < 0 || testX > 7) return false;
        auto copy = board;
        copy[kingY][testX].SetPiece(copy[kingY][kingX].GetPiece());
        copy[kingY][kingX].SetPiece(Piece(PieceType::None));
        if (isInCheck(color, copy)) return false;
    }

    return true;
}

// ----------------- Player Input -----------------

bool Game::OnLButtonDown(CPoint point) {
    selected_square = chessboard->OnLButtonDown(point);
    if (!selected_square) return false;

    // Reset selection highlight on all squares
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            chessboard -> GetSquare(y,x) -> SetSelected(false);

    if (piece_in_hand) {
        if (!isValidMove(*piece_in_hand, *selected_square, chessboard->GetSquares())) {
            piece_in_hand = nullptr; // reset so player can try another move
            return false;
        }

        int fromX, fromY, toX, toY;
        if (!findSquarePosition(*piece_in_hand, chessboard->GetSquares(), fromX, fromY) ||
            !findSquarePosition(*selected_square, chessboard->GetSquares(), toX, toY)) {
            piece_in_hand = nullptr;
            return false;
        }

        // --- simulate move for check ---
        auto copy = chessboard->GetSquares();

        Piece movingPiece = piece_in_hand->GetPiece();
        PieceColor moverColor = movingPiece.GetPieceColor();
        int dx = toX - fromX;
        int dy = toY - fromY;
        Piece targetPiece = selected_square->GetPiece();

        // perform move on copy
        copy[toY][toX].SetPiece(copy[fromY][fromX].GetPiece());
        copy[fromY][fromX].SetPiece(Piece(PieceType::None));

        // en passant simulation
        if (movingPiece.IsPawn() && targetPiece.GetPieceType() == PieceType::None && std::abs(dx) == 1 &&
            dy == ((moverColor == PieceColor::White) ? -1 : 1)) {
            int captureRow = toY + ((moverColor == PieceColor::White) ? 1 : -1);
            copy[captureRow][toX].SetPiece(Piece(PieceType::None));
        }

        // castling simulation
        if (movingPiece.IsKing() && std::abs(dx) == 2) {
            int rookFromX = (toX > fromX) ? 7 : 0;
            int rookToX = (toX > fromX) ? toX - 1 : toX + 1;
            int rookY = fromY;
            copy[rookY][rookToX].SetPiece(copy[rookY][rookFromX].GetPiece());
            copy[rookY][rookFromX].SetPiece(Piece(PieceType::None));
        }

        // illegal if king would still be in check
        if (isInCheck(moverColor, copy)) {
            piece_in_hand = nullptr;
            return false;
        }

        // --- real board updates ---
        // en passant capture
        if (movingPiece.IsPawn() && targetPiece.GetPieceType() == PieceType::None && std::abs(dx) == 1 &&
            dy == ((moverColor == PieceColor::White) ? -1 : 1)) {
            int captureRow = toY + ((moverColor == PieceColor::White) ? 1 : -1);
            chessboard->GetSquare(captureRow,toX) -> SetPiece(Piece(PieceType::None));
        }

        // move piece
        chessboard->GetSquare(toY,toX) -> SetPiece(piece_in_hand->GetPiece());
        chessboard->GetSquare(toY,toX) -> SetSelected(true);

        // mark moved
        {
            Piece moved = chessboard -> GetSquare(toY,toX) -> GetPiece();
            moved.SetHasMoved(true);
            chessboard->GetSquare(toY, toX) -> SetPiece(moved);
        }

        // clear origin
        chessboard->GetSquare(fromY,fromX) -> SetPiece(Piece(PieceType::None));

        // castling rook move
        if (movingPiece.IsKing() && std::abs(dx) == 2) {
            int rookFromX = (toX > fromX) ? 7 : 0;
            int rookToX = (toX > fromX) ? toX - 1 : toX + 1;
            int rookY = fromY;

            Piece rookPiece = chessboard->GetSquare(rookY,rookFromX) -> GetPiece();
            chessboard->GetSquare(rookY,rookToX) ->SetPiece(rookPiece);

            {
                Piece rp = chessboard->GetSquare(rookY,rookToX) ->GetPiece();
                rp.SetHasMoved(true);
                chessboard->GetSquare(rookY,rookToX) ->SetPiece(rp);
            }

            chessboard->GetSquare(rookY,rookFromX) ->SetPiece(Piece(PieceType::None));
            chessboard->GetSquare(rookY,rookToX) -> SetSelected(true);
        }

        // --- pawn promotion ---
        if (movingPiece.IsPawn()) {
            if ((moverColor == PieceColor::White && toY == 0) ||
                (moverColor == PieceColor::Black && toY == 7)) {
                parentDlg->PromotePawn(toY, toX);
            }
        }

        // reset held piece
        piece_in_hand = nullptr;

        // en passant tracking
        if (movingPiece.IsPawn() && std::abs(dy) == 2) {
            lastPawnDoubleStepX = toX;
            lastPawnDoubleStepY = toY;
        }
        else {
            lastPawnDoubleStepX = -1;
            lastPawnDoubleStepY = -1;
        }

        switchTurn();

        checkCheck();

        return true;
    }
    else {
        // pick up piece
        if (selected_square->GetPiece().GetPieceType() == PieceType::None) return false;
        if (selected_square->GetPiece().GetPieceColor() == currentPlayer->GetColor()) {
            piece_in_hand = selected_square;
            selected_square->SetSelected(true);
            return true;
        }
    }

    return false;
}

// ----------------- Movement Rules -----------------

bool Game::inBounds(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool Game::pathClear(int fromX, int fromY, int toX, int toY, const std::vector<std::vector<Square>>& board) {
    int dx = toX - fromX;
    int dy = toY - fromY;
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int x = fromX + stepX;
    int y = fromY + stepY;

    while (x != toX || y != toY) {
        if (board[y][x].GetPiece().GetPieceType() != PieceType::None) return false;
        x += stepX;
        y += stepY;
    }
    return true;
}

bool Game::isValidMove(Square& from, Square& to, const std::vector<std::vector<Square>>& board) {
    int fromX, fromY, toX, toY;
    if (!findSquarePosition(from, board, fromX, fromY) ||
        !findSquarePosition(to, board, toX, toY))
        return false;

    if (fromX == toX && fromY == toY) return false;

    Piece movingPiece = from.GetPiece();
    PieceColor moverColor = movingPiece.GetPieceColor();
    if (movingPiece.GetPieceType() == PieceType::None || moverColor == PieceColor::None) return false;

    Piece targetPiece = to.GetPiece();
    PieceColor targetColor = targetPiece.GetPieceColor();
    if (targetPiece.GetPieceType() != PieceType::None && targetColor == moverColor) return false;

    int dx = toX - fromX;
    int dy = toY - fromY;
    int adx = std::abs(dx);
    int ady = std::abs(dy);

    // --- Movement rules ---
    if (movingPiece.IsKing()) {
        if (max(adx, ady) == 1) return true; // normal king move

        // --- castling ---
        if (ady == 0 && adx == 2) {
            int rookX = (toX > fromX) ? 7 : 0;
            if (rookX < 0 || rookX > 7) return false;
            return canCastle(from, board[fromY][rookX], board);
        }
        return false;
    }
    if (movingPiece.IsQueen()) return (dx == 0 || dy == 0 || adx == ady) && pathClear(fromX, fromY, toX, toY, board);
    if (movingPiece.IsRook()) return (dx == 0 || dy == 0) && pathClear(fromX, fromY, toX, toY, board);
    if (movingPiece.IsBishop()) return (adx == ady) && pathClear(fromX, fromY, toX, toY, board);
    if (movingPiece.IsKnight()) return (adx == 1 && ady == 2) || (adx == 2 && ady == 1);

    if (movingPiece.IsPawn()) {
        int dir = (moverColor == PieceColor::White) ? -1 : 1;
        int startRow = (moverColor == PieceColor::White) ? 6 : 1;

        // capture
        if (dy == dir && std::abs(dx) == 1 && targetPiece.GetPieceType() != PieceType::None && targetColor != moverColor)
            return true;

        // forward
        if (dx == 0 && dy == dir && targetPiece.GetPieceType() == PieceType::None)
            return true;

        // double forward
        if (dx == 0 && dy == 2 * dir && fromY == startRow &&
            board[fromY + dir][fromX].GetPiece().GetPieceType() == PieceType::None &&
            targetPiece.GetPieceType() == PieceType::None)
            return true;

        // en passant
        int enPassantRow = (moverColor == PieceColor::White) ? 3 : 4;
        if (fromY == enPassantRow && dy == dir && std::abs(dx) == 1 && targetPiece.GetPieceType() == PieceType::None) {
            if (toX == lastPawnDoubleStepX && fromY == lastPawnDoubleStepY) return true;
        }

        return false;
    }

    return false;
}

// ----------------- Position / Check / Checkmate -----------------

bool Game::findSquarePosition(Square& target, const std::vector<std::vector<Square>>& board, int& outX, int& outY) {
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (&board[y][x] == &target) { outX = x; outY = y; return true; }
    return false;
}

bool Game::isInCheck(PieceColor kingColor, const std::vector<std::vector<Square>>& board) {
    Square* kingSquare = nullptr;
    for (int y = 0; y < 8 && !kingSquare; ++y)
        for (int x = 0; x < 8; ++x) {
            PieceType pt = board[y][x].GetPiece().GetPieceType();
            if ((kingColor == PieceColor::White && pt == PieceType::WhiteKing) ||
                (kingColor == PieceColor::Black && pt == PieceType::BlackKing)) {
                kingSquare = const_cast<Square*>(&board[y][x]); break;
            }
        }
    if (!kingSquare) return false;

    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x) {
            Piece p = board[y][x].GetPiece();
            if (p.GetPieceType() != PieceType::None && p.GetPieceColor() != kingColor)
                if (isValidMove(const_cast<Square&>(board[y][x]), *kingSquare, board)) 
                    return true;
        }
    return false;
}

bool Game::isCheckmate(PieceColor kingColor, const std::vector<std::vector<Square>>& board) {
    if (!isInCheck(kingColor, board)) return false;

    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x) {
            Piece p = board[y][x].GetPiece();
            if (p.GetPieceType() == PieceType::None || p.GetPieceColor() != kingColor) continue;

            for (int ty = 0; ty < 8; ++ty)
                for (int tx = 0; tx < 8; ++tx)
                    if (!(x == tx && y == ty) && isValidMove(const_cast<Square&>(board[y][x]), const_cast<Square&>(board[ty][tx]), board)) {
                        auto copy = board;
                        copy[ty][tx].SetPiece(copy[y][x].GetPiece());
                        copy[y][x].SetPiece(Piece(PieceType::None));
                        if (!isInCheck(kingColor, copy)) return false;
                    }
        }
    return true;
}

void Game::checkCheck() {
    PieceColor opponentColor = currentPlayer->GetColor();
    if (isInCheck(opponentColor, chessboard->GetSquares())) {
        if (isCheckmate(opponentColor, chessboard->GetSquares())) {
            int result = MessageBox(parentDlg->GetSafeHwnd(), CString(MAKEINTRESOURCE(IDS_CHECKMATE)), CString(MAKEINTRESOURCE(IDS_GAMEOVER)), MB_OK);
            if (result == IDOK) {
                parentDlg->RedrawGame();
                return;
            }
        }
        else {
            int result = MessageBox(parentDlg->GetSafeHwnd(), CString(MAKEINTRESOURCE(IDS_CHECK)), CString(MAKEINTRESOURCE(IDS_WARNING)), MB_OK);
            return;
        }
    }
}

// ----------------- Helpers -----------------

void Game::PromotePawn(Square* square, PieceType newType) {
    square->SetPiece(Piece(newType));
    PieceColor opponentColor = currentPlayer->GetColor();
    checkCheck();
}

Square* Game::GetChessboardSquare(int row, int col)
{
    return chessboard->GetSquare(row, col);
}
