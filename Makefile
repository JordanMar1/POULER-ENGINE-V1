##
## EPITECH PROJECT, 2026
## MY_GZDOOM
## File description:
## Makefile
##

CXX		=	g++
CXXFLAGS	=	-Wall -Wextra -Iinclude

LDFLAGS		=	-Linclude

LDLIBS		=	-larray -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio

SRCS		=	src/ErrorHandler.cpp \
				src/GameData.cpp \
				src/main.cpp \
				src/Settings.cpp \
				src/Window.cpp \
				src/Menu.cpp \
				src/Core.cpp \
				src/Maps.cpp \
				src/Game.cpp \
				src/Weapons.cpp \
				src/Renderer.cpp \
				src/Player.cpp \
				src/InterfaceItems.cpp \
				src/Head.cpp \
				src/Enemy.cpp

OBJS		=	$(SRCS:.cpp=.o)
NAME		=	pouler_or_not

.PHONY: all libarray clean fclean re

all: $(NAME)

libarray:
	@$(MAKE) -C include

$(NAME): libarray $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(MAKE) -C include clean
	@rm -f $(OBJS)

fclean: clean
	@$(MAKE) -C include fclean
	@rm -f $(NAME)
	@rm -f settings.conf

re:
	@$(MAKE) fclean
	@$(MAKE) all
