#ifndef BOARD_DATA_H
#define BOARD_DATA_H

#include <cstdint>
#include <cstdlib>

uint64_t bitl_tb[4][15] = {{
                               255ull,
                               65280ull,
                               16711680ull,
                               4278190080ull,
                               1095216660480ull,
                               280375465082880ull,
                               71776119061217280ull,
                               18374686479671623680ull,
                           },
                           {
                               72057594037927936ull,
                               144396663052566528ull,
                               288794425616760832ull,
                               577588855528488960ull,
                               1155177711073755136ull,
                               2310355422147575808ull,
                               4620710844295151872ull,
                               9241421688590303745ull,
                               36099303471055874ull,
                               141012904183812ull,
                               550831656968ull,
                               2151686160ull,
                               8405024ull,
                               32832ull,
                               128ull,
                           },
                           {
                               72340172838076673ull,
                               144680345676153346ull,
                               289360691352306692ull,
                               578721382704613384ull,
                               1157442765409226768ull,
                               2314885530818453536ull,
                               4629771061636907072ull,
                               9259542123273814144ull,
                           },
                           {
                               1ull,
                               258ull,
                               66052ull,
                               16909320ull,
                               4328785936ull,
                               1108169199648ull,
                               283691315109952ull,
                               72624976668147840ull,
                               145249953336295424ull,
                               290499906672525312ull,
                               580999813328273408ull,
                               1161999622361579520ull,
                               2323998145211531264ull,
                               4647714815446351872ull,
                               9223372036854775808ull,
                           }};

static inline constexpr uint64_t get_direction(uint64_t brd, int direction) {
    switch(direction) {
    case 0:
        return ((brd & 18374403900871474942ull) >> 1);
    case 1:
        return ((brd & 18374403900871474942ull) >> 9);
    case 2:
        return (brd >> 8);
    case 3:
        return ((brd & 9187201950435737471ull) >> 7);
    case 4:
        return ((brd & 9187201950435737471ull) << 1);
    case 5:
        return ((brd & 9187201950435737471ull) << 9);
    case 6:
        return (brd << 8);
    case 7:
        return ((brd & 18374403900871474942ull) << 7);
    default:
        abort();
    }
}

static inline constexpr uint64_t get_move_options(uint64_t brd) {
    uint64_t edge = ((brd & 18374403900871474942ull) >> 1) | ((brd & 9187201950435737471ull) << 1);
    edge |= ((brd & 18374403900871474942ull) >> 9) | (brd >> 8) | ((brd & 9187201950435737471ull) >> 7);
    edge |= ((brd & 18374403900871474942ull) << 7) | (brd << 8) | ((brd & 9187201950435737471ull) << 9);
    return edge & (~brd);
}

#endif
