#pragma once

class i_game_console
{
public:
	virtual			~i_game_console( ) { }
	virtual void	activate( ) = 0; // activates the console, makes it visible and brings it to the foreground
	virtual void	initialize( ) = 0;
	virtual void	hide( ) = 0; // hides the console
	virtual void	clear( ) = 0; // clears the console
	virtual bool	is_console_visible( ) = 0; // return true if the console has focus
	virtual void	set_parent( int parent ) = 0;
};
