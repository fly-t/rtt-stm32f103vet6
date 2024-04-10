#include "decode.h"
/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/

#include "decode.h"

static const uint16_t freq_tab[12]  = {262, 277, 294, 311, 330, 349, 369, 392, 415, 440, 466, 494}; //?????? CDEFGAB
static const uint8_t sign_tab[7]  = {0, 2, 4, 5, 7, 9, 11};     //1~7???????е?λ??
static const uint8_t length_tab[7] = {1, 2, 4, 8, 16, 32, 64};  //???????? 2^0,2^1,2^2,2^3...
static rt_uint16_t freq_tab_new[12];     //???????

//signature|????(0-11)       :  ??????????????????????;
//octachord|???????(-2??+2) :  < 0 ?????????; > 0 ?????????
static int beep_song_decode_new_freq(rt_uint8_t signature, rt_int8_t octachord)
{
    uint8_t i, j;
    for (i = 0; i < 12; i++)        // ????????????????????????????
    {
        j = i + signature;

        if (j > 11) //????????????????????????????????????
        {
            j = j - 12;
            freq_tab_new[i] = freq_tab[j] * 2;
        }
        else
        {
            freq_tab_new[i] = freq_tab[j];
        }

        /* ??????? */
        if (octachord < 0)
        {
            freq_tab_new[i] >>= (- octachord);
        }
        else if (octachord > 0)
        {
            freq_tab_new[i] <<= octachord; //????????? ????????
        }
    }
    return 0;
}

static int beep_song_decode(rt_uint16_t tone, rt_uint16_t length, rt_uint16_t *freq, rt_uint16_t *sound_len, rt_uint16_t *nosound_len)
{
    static const rt_uint16_t div0_len = SEMIBREVE_LEN;        // ??????????(ms)
    rt_uint16_t note_len, note_sound_len, current_freq;
    rt_uint8_t note, sharp, range, note_div, effect, dotted;

    note = tone % 10;                             //?????????
    range = tone / 10 % 10;                       //??????????
    sharp = tone / 100;                           //????????????

    current_freq = freq_tab_new[sign_tab[note - 1] + sharp]; //???????????????

    if (note != 0)
    {
        if (range == 1) current_freq >>= 1;       //???? ?????
        if (range == 3) current_freq <<= 1;       //???? ?????
        *freq = current_freq;
    }
    else
    {
        *freq = 0;
    }
    note_div = length_tab[length % 10];           //????????????

    effect = length / 10 % 10;                    //???????????(0???1????2????)
    dotted = length / 100;                        //???????

    note_len = div0_len / note_div;               //????????????

    if (dotted == 1)
        note_len = note_len + note_len / 2;

    if (effect != 1)
    {
        if (effect == 0)                          //??????????????????
        {
            note_sound_len = note_len * SOUND_SPACE;
        }
        else                                      //???????????????
        {
            note_sound_len = note_len / 2;
        }
    }
    else                                          //???????????????
    {
        note_sound_len = note_len;
    }
    if (note == 0)
    {
        note_sound_len = 0;
    }
    *sound_len = note_sound_len;

    *nosound_len = note_len - note_sound_len;     //??????????????

    return 0;
}

uint16_t beep_song_get_len(const struct beep_song *song)
{
    uint16_t cnt = 0;

    /* ??????0x00 0x00??β ?????????*/
    while (song->data[cnt])
    {
        cnt += 2;
    }
    return cnt / 2;
}

int beep_song_get_name(const struct beep_song *song, char *name)
{
    int i = 0;
    while (song->name[i])
    {
        name[i] = song->name[i];
        i++;
    }
    name[i] = '\0';
    return 0;
}

uint16_t beep_song_get_data(const struct beep_song *song, uint16_t index, struct beep_song_data *data)
{
    beep_song_decode(song->data[index * 2], song->data[index * 2 + 1], &data->freq, &data->sound_len, &data->nosound_len);

    return 2;
}

int beep_song_decode_init(void)
{
    beep_song_decode_new_freq(SOUND_SIGNATURE, SOUND_OCTACHORD);

    return 0;
}