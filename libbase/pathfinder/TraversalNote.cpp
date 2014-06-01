#include "TraversalNote.h"

TraversalNote::TraversalNote(void)
{
	reset();
}

TraversalNote::~TraversalNote(void)
{
}

void TraversalNote::reset()
{
	noteOpen = true;
	noteClosed = false;
	cost = 0;
	score = 0;
	parent = 0;
	point.x = point.y = 0;
}