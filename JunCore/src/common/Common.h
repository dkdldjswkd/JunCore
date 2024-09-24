#ifndef JUNCORE_COMMON_H
#define JUNCORE_COMMON_H

enum AccountTypes
{
    SEC_PLAYER = 0,
    SEC_MODERATOR = 1,
    SEC_GAMEMASTER = 2,
    SEC_ADMINISTRATOR = 3,
    SEC_CONSOLE = 4                                  // must be always last in list, accounts must have less security level always also
};

// todo : 삭제
enum LocaleConstant : uint8
{
    LOCALE_enUS = 0,
    LOCALE_koKR = 1,
    LOCALE_frFR = 2,
    LOCALE_deDE = 3,
    LOCALE_zhCN = 4,
    LOCALE_zhTW = 5,
    LOCALE_esES = 6,
    LOCALE_esMX = 7,
    LOCALE_ruRU = 8,

    TOTAL_LOCALES
};

void f_Common_h();

#endif