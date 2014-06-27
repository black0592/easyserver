#ifndef __DEFINE_H__
#define __DEFINE_H__

#ifdef WIN32
#ifndef bzero
#define bzero(data,size) memset(data, 0, size);
#endif
#endif



#endif		// __DEFINE_H__