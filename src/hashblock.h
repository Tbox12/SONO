#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "obj/sonoA/sph_skein.h"
#include "obj/sonoA/sph_cubehash.h"
#include "obj/sonoA/sph_fugue.h"
#include "obj/sonoA/sph_gost.h"
#include "obj/sonoA/raptor.h"
#include "obj/sonoA/sph_bmw.h"
#include "obj/sonoA/sph_jh.h"
#include "obj/sonoA/sph_keccak.h"


#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_skein512_context     z_skein;
GLOBAL sph_cubehash512_context  z_cubehash;
GLOBAL sph_fugue512_context     z_fugue;
GLOBAL sph_gost512_context      z_gost;
GLOBAL sph_bmw512_context       z_bmw;
GLOBAL sph_jh512_context        z_jh;
GLOBAL sph_keccak512_context    z_keccak;

#define fillz() do { \
    sph_skein512_init(&z_skein); \
    sph_cubehash512_init(&z_cubehash); \
    sph_fugue512_init(&z_fugue); \
    sph_gost512_init(&z_gost); \
    sph_bmw512_init(&z_bmw); \
    sph_jh512_init(&z_jh); \
    sph_keccak512_init(&z_keccak);\
} while (0) 

#define ZSKEIN (memcpy(&ctx_skein, &z_skein, sizeof(z_skein)))
#define ZFUGUE (memcpy(&ctx_fugue, &z_fugue, sizeof(z_fugue)))
#define ZGOST (memcpy(&ctx_gost, &z_gost, sizeof(z_gost)))
#define ZBMW (memcpy(&ctx_bmw, &z_bmw, sizeof(z_bmw)))
#define ZJH (memcpy(&ctx_jh, &z_jh, sizeof(z_jh)))
#define ZKECCAK (memcpy(&ctx_keccak, &z_keccak, sizeof(z_keccak)))


template<typename T1>
inline uint256 sonoA(const T1 pbegin, const T1 pend)

{
    sph_skein512_context        ctx_skein;
    sph_cubehash512_context     ctx_cubehash;
    sph_fugue512_context        ctx_fugue;
    sph_gost512_context         ctx_gost;
    sph_bmw512_context          ctx_bmw;
    sph_jh512_context           ctx_jh;
    sph_keccak512_context       ctx_keccak;

    static unsigned char pblank[1];

#ifndef QT_NO_DEBUG
    //std::string strhash;
    //strhash = "";
#endif
    
    uint512 hash[17];
    
    sph_cubehash512_init(&ctx_cubehash);
    sph_cubehash512 (&ctx_cubehash, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&hash[0]));
        
    sph_skein512_init(&ctx_skein);
    sph_skein512 (&ctx_skein, static_cast<const void*>(&hash[0]), 64);
    sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[1]));

    sph_fugue512_init(&ctx_fugue);
    sph_fugue512 (&ctx_fugue, static_cast<const void*>(&hash[1]), 64);
    sph_fugue512_close(&ctx_fugue, static_cast<void*>(&hash[2]));

    sph_gost512_init(&ctx_gost);
    sph_gost512 (&ctx_gost, static_cast<const void*>(&hash[2]), 64);
    sph_gost512_close(&ctx_gost, static_cast<void*>(&hash[3]));

    sph_bmw512_init(&ctx_bmw);
    sph_bmw512 (&ctx_bmw, static_cast<const void*>(&hash[3]), 64);
    sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[4]));

    sph_jh512_init(&ctx_jh);
    sph_jh512 (&ctx_jh, static_cast<const void*>(&hash[4]), 64);
    sph_jh512_close(&ctx_jh, static_cast<void*>(&hash[5]));

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hash[5]), 64);
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[6]));


    return hash[3].trim256();
}

#endif // HASHBLOCK_H
