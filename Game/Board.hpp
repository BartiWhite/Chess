#ifndef Board_hpp
#define Board_hpp

#endif /* Board_hpp */

//                    if(map[y][x] != type)
//                    {
//                        dopler->updateTexture(map[y][x]);
//                        dopler->Update(x*90 + 5, y*90 + 5);
//                    }
//                    type = map[y][x];
//                    renderPosibleMoves(y, x); // update queen's position on map
//                    lastPosition = y*10 + x;
//
//                    if(map[y][x] == 5)
//                    {
//                        inBoundries = true;
//                        selected = &piecess[14]->dirRec;
//                        clickOffset.x = mousePos.x - piecess[14]->dirRec.x;
//                        clickOffset.y = mousePos.y - piecess[14]->dirRec.y;
//                    }
//                    else if(map[y][x] == 7)
//                    {
//                        inBoundries = true;
//                        selected = &piecess[22]->dirRec;
//                        clickOffset.x = mousePos.x - piecess[22]->dirRec.x;
//                        clickOffset.y = mousePos.y - piecess[22]->dirRec.y;
//                    }
            
//                if(map[y][x] == 5)
//                {
//                    if(map[y][x] != type)
//                    {
//                        dopler->updateTexture(map[y][x]);
//                        dopler->Update(x*90 + 5, y*90 + 5);
//                    }
//                    type = map[y][x];
//                    renderPosibleMoves(y, x); // update queen's position on map
//                    lastPosition = y*10 + x;
//                }
//                else if(map[y][x] == 7)
//                {
//                    if(map[y][x] != type)
//                    {
//                        dopler->updateTexture(map[y][x]);
//                        dopler->Update(x*90 + 5, y*90 + 5);
//                    }
//                    type = map[y][x];
//                    renderPosibleMoves(y, x); // update queen's position on map
//                    lastPosition = y*10 + x;
//                }






//void Game::whiteCheckCheckAndPin(int y, int x) // shorten code a bit
//{
//    bool tmpCheck = check;
//    int cover = 0;
//    int tmpCover = 0;
//    bool breakOut = false;
//    int piece = 0;
//    //int mouseX = mousePos.x;
//    //int mouseY = mousePos.y;
//    //std::cout << "check\n";
//
//    // looking for check in horizontal
//    for(int i = 1; i < 8-x; i++)
//    {
//        piece = this->map[y][x+i];
//        if(piece > 6 && piece != 11 && piece != 8)
//            break;
//        if(piece <= 6 && piece > 0)
//        {
//            cover++;
//            if(cover > 1)
//                break;
//            tmpCover = y*10 + x+i;
//        }
//        else if(piece == 8)
//        {
//            if(cover == 0)
//            {
//                for(int j = 0; j < 2; j++)
//                    if(piecess[24 + j]->dirRec.y/90 == y && piecess[24 + j]->dirRec.x/90 == x + i && piecess[24 + j]->getAlive())
//                    {
//                        piecess[24 + j]->setIsAttacking(true);
//                        check = true;
//                        breakOut = true;
//                        break;
//                    }
//            }
//            else if(cover == 1)
//            {
//                for(int j = 0; j < 15; j++)
//                    if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                    {
//                        piecess[j]->setIsPined(true);
//                        breakOut = true;
//                        break;
//                    }
//            }
//        }
//        else if(piece == 11 && piecess[30]->getAlive())
//        {
//            if(cover == 0)
//            {
//                check = true;
//                piecess[30]->setIsAttacking(true);
//                breakOut = true;
//            }
//            else if(cover == 1)
//            {
//                for(int j = 0; j < 15; j++)
//                {
//                    if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                    {
//                        piecess[j]->setIsPined(true);
//                        breakOut = true;
//                        break;
//                    }
//                }
//            }
//        }
//        if(breakOut == true)
//            break;
//        if(cover > 1)
//            break;
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//    if(x > 0)
//    {
//        for(int i = 1; i <= x; i++)
//        {
//            piece = this->map[y][x-i];
//            if(piece > 6 && piece != 11 && piece != 8)
//                break;
//            if(piece <= 6 && piece > 0)
//            {
//                cover++;
//                if(cover > 1)
//                    break;
//                tmpCover = y*10 + x-i;
//            }
//            else if(piece == 8)
//            {
//                if(cover == 0)
//                {
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[24 + j]->dirRec.x/90 == x - i && piecess[24 + j]->dirRec.y/90 == y && piecess[24 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[24 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 11 && piecess[30]->getAlive())
//            {
//                if(cover == 0)
//                {
//                    check = true;
//                    piecess[30]->setIsAttacking(true);
//                    breakOut = true;
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10)
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            if(breakOut == true)
//                break;
//            if(cover > 1)
//                break;
//        }
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//
//    // looking for check in vertical
//    for(int i = 1; i < 8-y; i++)
//    {
//        piece = this->map[y+i][x];
//        if(piece > 6 && piece != 11 && piece != 8)
//            break;
//        if(piece <= 6 && piece > 0)
//        {
//            cover++;
//            if(cover > 1)
//                break;
//            tmpCover = (y+i)*10 + x;
//        }
//        else if(piece == 8)
//        {
//            if(cover == 0)
//            {
//                for(int j = 0; j < 2; j++)
//                    if(piecess[24 + j]->dirRec.y/90 == y + i && piecess[24 + j]->dirRec.x/90 == x && piecess[24 + j]->getAlive())
//                    {
//                        piecess[24 + j]->setIsAttacking(true);
//                        check = true;
//                        breakOut = true;
//                        break;
//                    }
//            }
//            else if(cover == 1)
//            {
//                for(int j = 0; j < 15; j++)
//                    if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                    {
//                        piecess[j]->setIsPined(true);
//                        breakOut = true;
//                        break;
//                    }
//            }
//        }
//        else if(piece == 11 && piecess[30]->getAlive())
//        {
//            if(cover == 0)
//            {
//                check = true;
//                piecess[30]->setIsAttacking(true);
//                breakOut = true;
//            }
//            else if(cover == 1)
//            {
//                for(int j = 0; j < 15; j++)
//                {
//                    if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                    {
//                        piecess[j]->setIsPined(true);
//                        breakOut = true;
//                        break;
//                    }
//                }
//            }
//        }
//        if(breakOut == true)
//            break;
//        if(cover > 1)
//            break;
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//    if(y > 0)
//    {
//        for(int i = 1; i <= y; i++)
//        {
//            piece = this->map[y-i][x];
//            if(piece > 6 && piece != 11 && piece != 8)
//                break;
//            if(piece <= 6 && piece > 0)
//            {
//                cover++;
//                if(cover > 1)
//                    break;
//                tmpCover = (y-i)*10 + x;
//            }
//            else if(piece == 8)
//            {
//                if(cover == 0)
//                {
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[24 + j]->dirRec.x/90 == x && piecess[24 + j]->dirRec.y/90 == y - i && piecess[24 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[24 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 11 && piecess[30]->getAlive())
//            {
//                if(cover == 0)
//                {
//                    check = true;
//                    piecess[30]->setIsAttacking(true);
//                    breakOut = true;
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10)
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            if(breakOut == true)
//                break;
//            if(cover > 1)
//                break;
//        }
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//
//    // looking for check in diagonals
//    // down and right
//    int lenght = 0;
//    if(x < 7 && y < 7)
//    {
//        lenght = 8 - x >= 8 - y ? 8 - y : 8 - x;
//        for(int i = 1; i < lenght; i++)
//        {
//            piece = this->map[y+i][x+i];
//            if(piece > 6 && piece != 11 && piece != 10)
//                break;
//            if(piece <= 6 && piece > 0)
//            {
//                cover++;
//                if(cover > 1)
//                    break;
//                tmpCover = (y+i)*10 + x+i;
//            }
//            else if(piece == 10)
//            {
//                if(cover == 0)
//                {
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[28 + j]->dirRec.x/90 == x + i && piecess[28 + j]->dirRec.y/90 == y + i && piecess[28 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[28 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 11 && piecess[30]->getAlive())
//            {
//                if(cover == 0)
//                {
//                    check = true;
//                    piecess[30]->setIsAttacking(true);
//                    breakOut = true;
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10)
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            if(breakOut == true)
//                break;
//            if(cover > 1)
//                break;
//        }
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//    lenght = 0;
//    // up and left
//    if(0 < x && 0 < y)
//    {
//        lenght = x >= y ? y+1 : x+1;
//        for(int i = 1; i < lenght; i++)
//        {
//            piece = this->map[y-i][x-i];
//            if(piece > 7 && piece != 11 && piece != 10)
//                break;
//            if(piece <= 6 && piece > 0)
//            {
//                cover++;
//                if(cover > 1)
//                    break;
//                tmpCover = (y-i)*10 + x-i;
//            }
//            else if(piece == 10)
//            {
//                if(cover == 0)
//                {
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[28 + j]->dirRec.x/90 == x - i && piecess[28 + j]->dirRec.y/90 == y - i && piecess[28 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[28 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 11 && piecess[30]->getAlive())
//            {
//                if(cover == 0)
//                {
//                    check = true;
//                    piecess[30]->setIsAttacking(true);
//                    breakOut = true;
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10)
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 7)
//            {
//                if(i == 1)
//                    for(int j = 0; j < 8; j++)
//                        if(piecess[16 + j]->dirRec.x/90 == x - i && piecess[16 + j]->dirRec.y/90 == y - i && piecess[16 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[16 + j]->setIsAttacking(true);
//                            break;
//                        }
//                breakOut = true;
//            }
//            if(breakOut == true)
//                break;
//            if(cover > 1)
//                break;
//        }
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//    lenght = 0;
//    // down and left
//    if(0 < x && y < 7)
//    {
//        lenght = x >= 8-y ? 8-y : x+1;
//        for(int i = 1; i < lenght; i++)
//        {
//            piece = this->map[y+i][x-i];
//            if(piece > 6 && piece != 11 && piece != 10)
//                break;
//            if(piece <= 6 && piece > 0)
//            {
//                cover++;
//                if(cover > 1)
//                    break;
//                tmpCover = (y+i)*10 + x-i;
//            }
//            else if(piece == 10)
//            {
//                if(cover == 0)
//                {
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[28 + j]->dirRec.x/90 == x - i && piecess[28 + j]->dirRec.y/90 == y + i && piecess[28 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[28 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 11 && piecess[30]->getAlive())
//            {
//                if(cover == 0)
//                {
//                    check = true;
//                    piecess[30]->setIsAttacking(true);
//                    breakOut = true;
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10)
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            if(breakOut == true)
//                break;
//            if(cover > 1)
//                break;
//        }
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//    lenght = 0;
//    // up and right
//    if(x < 7 && 0 < y)
//    {
//        lenght = y >= 8-x ? 8-x : y+1;
//        for(int i = 1; i < lenght; i++)
//        {
//            piece = this->map[y-i][x+i];
//            if(piece > 7 && piece != 11 && piece != 10)
//                break;
//            if(piece <= 6 && piece > 0)
//            {
//                cover++;
//                if(cover > 1)
//                    break;
//                tmpCover = (y-i)*10 + x+i;
//            }
//            else if(piece == 10)
//            {
//                if(cover == 0)
//                {
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[28 + j]->dirRec.x/90 == x + i && piecess[28 + j]->dirRec.y/90 == y - i && piecess[28 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[28 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10 && piecess[j]->getAlive())
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 11 && piecess[30]->getAlive())
//            {
//                if(cover == 0)
//                {
//                    check = true;
//                    piecess[30]->setIsAttacking(true);
//                    breakOut = true;
//                }
//                else if(cover == 1)
//                {
//                    for(int j = 0; j < 15; j++)
//                        if(piecess[j]->dirRec.y/90 == tmpCover/10 && piecess[j]->dirRec.x/90 == tmpCover%10)
//                        {
//                            piecess[j]->setIsPined(true);
//                            breakOut = true;
//                            break;
//                        }
//                }
//            }
//            else if(piece == 7)
//            {
//                if(i == 1)
//                    for(int j = 0; j < 8; j++)
//                        if(piecess[16 + j]->dirRec.x/90 == x + i && piecess[16 + j]->dirRec.y/90 == y - i && piecess[16 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[16 + j]->setIsAttacking(true);
//                            break;
//                        }
//                breakOut = true;
//            }
//            if(breakOut == true)
//                break;
//            if(cover > 1)
//                break;
//        }
//    }
//    cover = 0;
//    tmpCover = 0;
//    breakOut = false;
//    lenght = 0;
//
//    if(y > 0)
//    {
//        if(y > 1)
//        {
//            if(x > 0)
//            {
//                if(this->map[y-2][x-1] == 9)
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[26 + j]->dirRec.x/90 == x - 1 && piecess[26 + j]->dirRec.y/90 == y - 2 && piecess[26 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[26 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//            }
//            if(x < 7 && !breakOut)
//            {
//                if(this->map[y-2][x+1] == 9)
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[26 + j]->dirRec.x/90 == x + 1 && piecess[26 + j]->dirRec.y/90 == y - 2 && piecess[26 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[26 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//            }
//        }
//
//        if(x > 1 && !breakOut)
//        {
//            if(this->map[y-1][x-2] == 9)
//                for(int j = 0; j < 2; j++)
//                    if(piecess[26 + j]->dirRec.x/90 == x - 2 && piecess[26 + j]->dirRec.y/90 == y - 1 && piecess[26 + j]->getAlive())
//                    {
//                        check = true;
//                        piecess[26 + j]->setIsAttacking(true);
//                        breakOut = true;
//                        break;
//                    }
//        }
//        if(x < 6 && !breakOut)
//        {
//            if(this->map[y-1][x+2] == 9)
//                for(int j = 0; j < 2; j++)
//                    if(piecess[26 + j]->dirRec.x/90 == x + 2 && piecess[26 + j]->dirRec.y/90 == y - 1 && piecess[26 + j]->getAlive())
//                    {
//                        check = true;
//                        piecess[26 + j]->setIsAttacking(true);
//                        breakOut = true;
//                        break;
//                    }
//        }
//
//    }
//    if(y < 7 && !breakOut)
//    {
//        if(y < 6)
//        {
//            if(x > 0)
//            {
//                if(this->map[y+2][x-1] == 9)
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[26 + j]->dirRec.x/90 == x - 1 && piecess[26 + j]->dirRec.y/90 == y + 2 && piecess[26 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[26 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//            }
//            if(x < 7 && !breakOut)
//            {
//                if(this->map[y+2][x+1] == 9)
//                    for(int j = 0; j < 2; j++)
//                        if(piecess[26 + j]->dirRec.x/90 == x + 1 && piecess[26 + j]->dirRec.y/90 == y + 2 && piecess[26 + j]->getAlive())
//                        {
//                            check = true;
//                            piecess[26 + j]->setIsAttacking(true);
//                            breakOut = true;
//                            break;
//                        }
//            }
//        }
//
//        if(x > 1 && !breakOut)
//        {
//            if(this->map[y+1][x-2] == 9)
//                for(int j = 0; j < 2; j++)
//                    if(piecess[26 + j]->dirRec.x/90 == x - 2 && piecess[26 + j]->dirRec.y/90 == y + 1 && piecess[26 + j]->getAlive())
//                    {
//                        check = true;
//                        piecess[26 + j]->setIsAttacking(true);
//                        breakOut = true;
//                        break;
//                    }
//        }
//        if(x < 6 && !breakOut)
//        {
//            if(this->map[y+1][x+2] == 9)
//                for(int j = 0; j < 2; j++)
//                    if(piecess[26 + j]->dirRec.x/90 == x + 2 && piecess[26 + j]->dirRec.y/90 == y + 1 && piecess[26 + j]->getAlive())
//                    {
//                        check = true;
//                        piecess[26 + j]->setIsAttacking(true);
//                        breakOut = true;
//                        break;
//                    }
//        }
//    }
//    breakOut = false;
//
//    if(tmpCheck == check)
//        check = false;
//}


// down and right
//        else if(relativePosition == 0)
//        {
//            if(xDiv > 0 && yDiv > 0)
//            {
//                int lenght = 7 - x < 7 - y ? 7 - x : 7 - y;
//                for(int i = 1; i <= lenght; i++)
//                {
//                    int piece = map[(y + i)*width + x + i];
//
//                    if(0 < piece && piece < 29)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                    else if(28 < piece && piece < 32)
//                    {
//                        for(int j = 1; j <= xDiv; j++)
//                        {
//                            piece = map[(y - j)*width + x - j];
//                            if(0 < piece && piece != 16)
//                            {
//                                blackEnPassantPosition = x;
//                                break;
//                            }
//                            else if(piece == 16)
//                                break;
//
//                        }
//                        break;
//                    }
//                    else if(i == lenght)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                }
//            }
//            // up and left
//            else if(xDiv < 0 && yDiv < 0)
//            {
//                int lenght = x < y ? x : y;
//                for(int i = 1; i <= lenght; i++)
//                {
//                    int piece = map[(y - i)*width + x - i];
//
//                    if(0 < piece && piece < 29)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                    else if(28 < piece && piece < 32)
//                    {
//                        for(int j = 1; j <= -xDiv; j++)
//                        {
//                            piece = map[(y + j)*width + x + j];
//                            if(0 < piece && piece != 16)
//                            {
//                                blackEnPassantPosition = x;
//                                break;
//                            }
//                            else if(piece == 16)
//                                break;
//                        }
//                        break;
//                    }
//                    else if(i == lenght)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                }
//            }
//            // down and left
//            else if(xDiv < 0 && yDiv > 0)
//            {
//                int lenght = x < (7 - y) ? x : 7 - y;
//                for(int i = 1; i <= lenght; i++)
//                {
//                    int piece = map[(y + i)*width + x - i];
//
//                    if(0 < piece && piece < 29)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                    else if(28 < piece && piece < 32)
//                    {
//                        for(int j = 1; j <= -xDiv; j++)
//                        {
//                            piece = map[(y - j)*width + x + j];
//                            if(0 < piece && piece != 16)
//                            {
//                                blackEnPassantPosition = x;
//                                break;
//                            }
//                            else if(piece == 16)
//                                break;
//                        }
//                        break;
//                    }
//                    else if(i == lenght)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                }
//            }
//            // up and right
//            else if(xDiv > 0 && yDiv < 0)
//            {
//                int lenght = 7 - x < y ? 7 - x : y;
//                for(int i = 1; i <= lenght; i++)
//                {
//                    int piece = map[(y - i)*width + x + i];
//
//                    if(0 < piece && piece < 29)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                    else if(28 < piece && piece < 32)
//                    {
//                        for(int j = 1; j <= xDiv; j++)
//                        {
//                            piece = map[(y + j)*width + x - j];
//                            if(0 < piece && piece != 16)
//                            {
//                                blackEnPassantPosition = x;
//                                break;
//                            }
//                            else if(piece == 16)
//                                break;
//                        }
//                        break;
//                    }
//                    else if(i == lenght)
//                    {
//                        blackEnPassantPosition = x;
//                        break;
//                    }
//                }
//            }
//            else blackEnPassantPosition = x;
//        }
