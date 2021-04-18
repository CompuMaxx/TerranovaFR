const struct PokedexEntry gPokedexEntries[] =
{
    [NATIONAL_DEX_NONE] =
    {
        .categoryName = _("Unknown"),
        .categoryNameSpa = _("Descon."),
        .height = 0,
        .weight = 0,
        .description = gDummyPokedexText,
        .descriptionSpa = gDummyPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BULBASAUR] =
    {
        .categoryName = _("Seed"),
        .categoryNameSpa = _("Semilla"),
        .height = 7,
        .weight = 69,
        .description = gBulbasaurPokedexText,
        .descriptionSpa = gBulbasaurPokedexTextSpa,
        .pokemonScale = 356,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_IVYSAUR] =
    {
        .categoryName = _("Seed"),
        .categoryNameSpa = _("Semilla"),
        .height = 10,
        .weight = 130,
        .description = gIvysaurPokedexText,
        .descriptionSpa = gIvysaurPokedexTextSpa,
        .pokemonScale = 332,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VENUSAUR] =
    {
        .categoryName = _("Seed"),
        .categoryNameSpa = _("Semilla"),
        .height = 20,
        .weight = 1000,
        .description = gVenusaurPokedexText,
        .descriptionSpa = gVenusaurPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 375,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_CHARMANDER] =
    {
        .categoryName = _("Lizard"),
        .categoryNameSpa = _("Lagartija"),
        .height = 6,
        .weight = 85,
        .description = gCharmanderPokedexText,
        .descriptionSpa = gCharmanderPokedexTextSpa,
        .pokemonScale = 410,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CHARMELEON] =
    {
        .categoryName = _("Flame"),
        .categoryNameSpa = _("Llama"),
        .height = 11,
        .weight = 190,
        .description = gCharmeleonPokedexText,
        .descriptionSpa = gCharmeleonPokedexTextSpa,
        .pokemonScale = 294,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CHARIZARD] =
    {
        .categoryName = _("Flame"),
        .categoryNameSpa = _("Llama"),
        .height = 17,
        .weight = 905,
        .description = gCharizardPokedexText,
        .descriptionSpa = gCharizardPokedexTextSpa,
        .pokemonScale = 271,
        .pokemonOffset = 0,
        .trainerScale = 317,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_SQUIRTLE] =
    {
        .categoryName = _("Tiny Turtle"),
        .categoryNameSpa = _("Tortuguita"),
        .height = 5,
        .weight = 90,
        .description = gSquirtlePokedexText,
        .descriptionSpa = gSquirtlePokedexTextSpa,
        .pokemonScale = 412,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WARTORTLE] =
    {
        .categoryName = _("Turtle"),
        .categoryNameSpa = _("Tortuga"),
        .height = 10,
        .weight = 225,
        .description = gWartortlePokedexText,
        .descriptionSpa = gWartortlePokedexTextSpa,
        .pokemonScale = 334,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BLASTOISE] =
    {
        .categoryName = _("Shellfish"),
        .categoryNameSpa = _("Marisco"),
        .height = 16,
        .weight = 855,
        .description = gBlastoisePokedexText,
        .descriptionSpa = gBlastoisePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 329,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_CATERPIE] =
    {
        .categoryName = _("Worm"),
        .categoryNameSpa = _("Gusano"),
        .height = 3,
        .weight = 29,
        .description = gCaterpiePokedexText,
        .descriptionSpa = gCaterpiePokedexTextSpa,
        .pokemonScale = 549,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_METAPOD] =
    {
        .categoryName = _("Cocoon"),
        .categoryNameSpa = _("Capullo"),
        .height = 7,
        .weight = 99,
        .description = gMetapodPokedexText,
        .descriptionSpa = gMetapodPokedexTextSpa,
        .pokemonScale = 350,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BUTTERFREE] =
    {
        .categoryName = _("Butterfly"),
        .categoryNameSpa = _("Mariposa"),
        .height = 11,
        .weight = 320,
        .description = gButterfreePokedexText,
        .descriptionSpa = gButterfreePokedexTextSpa,
        .pokemonScale = 312,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WEEDLE] =
    {
        .categoryName = _("Hairy Bug"),
        .categoryNameSpa = _("Oruga"),
        .height = 3,
        .weight = 32,
        .description = gWeedlePokedexText,
        .descriptionSpa = gWeedlePokedexTextSpa,
        .pokemonScale = 455,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KAKUNA] =
    {
        .categoryName = _("Cocoon"),
        .categoryNameSpa = _("Capullo"),
        .height = 6,
        .weight = 100,
        .description = gKakunaPokedexText,
        .descriptionSpa = gKakunaPokedexTextSpa,
        .pokemonScale = 424,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BEEDRILL] =
    {
        .categoryName = _("Poison Bee"),
        .categoryNameSpa = _("Aveja Ven."),
        .height = 10,
        .weight = 295,
        .description = gBeedrillPokedexText,
        .descriptionSpa = gBeedrillPokedexTextSpa,
        .pokemonScale = 366,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PIDGEY] =
    {
        .categoryName = _("Tiny Bird"),
        .categoryNameSpa = _("Pajarito"),
        .height = 3,
        .weight = 18,
        .description = gPidgeyPokedexText,
        .descriptionSpa = gPidgeyPokedexTextSpa,
        .pokemonScale = 492,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PIDGEOTTO] =
    {
        .categoryName = _("Bird"),
        .categoryNameSpa = _("Pájaro"),
        .height = 11,
        .weight = 300,
        .description = gPidgeottoPokedexText,
        .descriptionSpa = gPidgeottoPokedexTextSpa,
        .pokemonScale = 334,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PIDGEOT] =
    {
        .categoryName = _("Bird"),
        .categoryNameSpa = _("Pájaro"),
        .height = 15,
        .weight = 395,
        .description = gPidgeotPokedexText,
        .descriptionSpa = gPidgeotPokedexTextSpa,
        .pokemonScale = 269,
        .pokemonOffset = -2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RATTATA] =
    {
        .categoryName = _("Mouse"),
        .categoryNameSpa = _("Ratón"),
        .height = 3,
        .weight = 35,
        .description = gRattataPokedexText,
        .descriptionSpa = gRattataPokedexTextSpa,
        .pokemonScale = 481,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RATICATE] =
    {
        .categoryName = _("Mouse"),
        .categoryNameSpa = _("Ratón"),
        .height = 7,
        .weight = 185,
        .description = gRaticatePokedexText,
        .descriptionSpa = gRaticatePokedexTextSpa,
        .pokemonScale = 401,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SPEAROW] =
    {
        .categoryName = _("Tiny Bird"),
        .categoryNameSpa = _("Pajarito"),
        .height = 3,
        .weight = 20,
        .description = gSpearowPokedexText,
        .descriptionSpa = gSpearowPokedexTextSpa,
        .pokemonScale = 571,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FEAROW] =
    {
        .categoryName = _("Beak"),
        .categoryNameSpa = _("Pico"),
        .height = 12,
        .weight = 380,
        .description = gFearowPokedexText,
        .descriptionSpa = gFearowPokedexTextSpa,
        .pokemonScale = 282,
        .pokemonOffset = -1,
        .trainerScale = 272,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_EKANS] =
    {
        .categoryName = _("Snake"),
        .categoryNameSpa = _("Serpiente"),
        .height = 20,
        .weight = 69,
        .description = gEkansPokedexText,
        .descriptionSpa = gEkansPokedexTextSpa,
        .pokemonScale = 298,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ARBOK] =
    {
        .categoryName = _("Cobra"),
        .categoryNameSpa = _("Cobra"),
        .height = 35,
        .weight = 650,
        .description = gArbokPokedexText,
        .descriptionSpa = gArbokPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 296,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_PIKACHU] =
    {
        .categoryName = _("Mouse"),
        .categoryNameSpa = _("Ratón"),
        .height = 4,
        .weight = 60,
        .description = gPikachuPokedexText,
        .descriptionSpa = gPikachuPokedexTextSpa,
        .pokemonScale = 479,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RAICHU] =
    {
        .categoryName = _("Mouse"),
        .categoryNameSpa = _("Ratón"),
        .height = 8,
        .weight = 300,
        .description = gRaichuPokedexText,
        .descriptionSpa = gRaichuPokedexTextSpa,
        .pokemonScale = 426,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SANDSHREW] =
    {
        .categoryName = _("Mouse"),
        .categoryNameSpa = _("Ratón"),
        .height = 6,
        .weight = 120,
        .description = gSandshrewPokedexText,
        .descriptionSpa = gSandshrewPokedexTextSpa,
        .pokemonScale = 370,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SANDSLASH] =
    {
        .categoryName = _("Mouse"),
        .categoryNameSpa = _("Ratón"),
        .height = 10,
        .weight = 295,
        .description = gSandslashPokedexText,
        .descriptionSpa = gSandslashPokedexTextSpa,
        .pokemonScale = 341,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NIDORAN_F] =
    {
        .categoryName = _("Poison Pin"),
        .categoryNameSpa = _("Pin Veneno"),
        .height = 4,
        .weight = 70,
        .description = gNidoranFPokedexText,
        .descriptionSpa = gNidoranFPokedexTextSpa,
        .pokemonScale = 488,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NIDORINA] =
    {
        .categoryName = _("Poison Pin"),
        .categoryNameSpa = _("Pin Veneno"),
        .height = 8,
        .weight = 200,
        .description = gNidorinaPokedexText,
        .descriptionSpa = gNidorinaPokedexTextSpa,
        .pokemonScale = 381,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NIDOQUEEN] =
    {
        .categoryName = _("Drill"),
        .categoryNameSpa = _("Taladro"),
        .height = 13,
        .weight = 600,
        .description = gNidoqueenPokedexText,
        .descriptionSpa = gNidoqueenPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NIDORAN_M] =
    {
        .categoryName = _("Poison Pin"),
        .categoryNameSpa = _("Pin Veneno"),
        .height = 5,
        .weight = 90,
        .description = gNidoranMPokedexText,
        .descriptionSpa = gNidoranMPokedexTextSpa,
        .pokemonScale = 480,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NIDORINO] =
    {
        .categoryName = _("Poison Pin"),
        .categoryNameSpa = _("Pin Veneno"),
        .height = 9,
        .weight = 195,
        .description = gNidorinoPokedexText,
        .descriptionSpa = gNidorinoPokedexTextSpa,
        .pokemonScale = 408,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NIDOKING] =
    {
        .categoryName = _("Drill"),
        .categoryNameSpa = _("Taladro"),
        .height = 14,
        .weight = 620,
        .description = gNidokingPokedexText,
        .descriptionSpa = gNidokingPokedexTextSpa,
        .pokemonScale = 304,
        .pokemonOffset = 3,
        .trainerScale = 323,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_CLEFAIRY] =
    {
        .categoryName = _("Fairy"),
        .categoryNameSpa = _("Hada"),
        .height = 6,
        .weight = 75,
        .description = gClefairyPokedexText,
        .descriptionSpa = gClefairyPokedexTextSpa,
        .pokemonScale = 425,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CLEFABLE] =
    {
        .categoryName = _("Fairy"),
        .categoryNameSpa = _("Hada"),
        .height = 13,
        .weight = 400,
        .description = gClefablePokedexText,
        .descriptionSpa = gClefablePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 272,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_VULPIX] =
    {
        .categoryName = _("Fox"),
        .categoryNameSpa = _("Zorro"),
        .height = 6,
        .weight = 99,
        .description = gVulpixPokedexText,
        .descriptionSpa = gVulpixPokedexTextSpa,
        .pokemonScale = 497,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NINETALES] =
    {
        .categoryName = _("Fox"),
        .categoryNameSpa = _("Zorro"),
        .height = 11,
        .weight = 199,
        .description = gNinetalesPokedexText,
        .descriptionSpa = gNinetalesPokedexTextSpa,
        .pokemonScale = 339,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_JIGGLYPUFF] =
    {
        .categoryName = _("Balloon"),
        .categoryNameSpa = _("Globo"),
        .height = 5,
        .weight = 55,
        .description = gJigglypuffPokedexText,
        .descriptionSpa = gJigglypuffPokedexTextSpa,
        .pokemonScale = 419,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WIGGLYTUFF] =
    {
        .categoryName = _("Balloon"),
        .categoryNameSpa = _("Globo"),
        .height = 10,
        .weight = 120,
        .description = gWigglytuffPokedexText,
        .descriptionSpa = gWigglytuffPokedexTextSpa,
        .pokemonScale = 328,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ZUBAT] =
    {
        .categoryName = _("Bat"),
        .categoryNameSpa = _("Murciélago"),
        .height = 8,
        .weight = 75,
        .description = gZubatPokedexText,
        .descriptionSpa = gZubatPokedexTextSpa,
        .pokemonScale = 355,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GOLBAT] =
    {
        .categoryName = _("Bat"),
        .categoryNameSpa = _("Murciélago"),
        .height = 16,
        .weight = 550,
        .description = gGolbatPokedexText,
        .descriptionSpa = gGolbatPokedexTextSpa,
        .pokemonScale = 291,
        .pokemonOffset = 0,
        .trainerScale = 296,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_ODDISH] =
    {
        .categoryName = _("Weed"),
        .categoryNameSpa = _("Hierbajo"),
        .height = 5,
        .weight = 54,
        .description = gOddishPokedexText,
        .descriptionSpa = gOddishPokedexTextSpa,
        .pokemonScale = 423,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GLOOM] =
    {
        .categoryName = _("Weed"),
        .categoryNameSpa = _("Hierbajo"),
        .height = 8,
        .weight = 86,
        .description = gGloomPokedexText,
        .descriptionSpa = gGloomPokedexTextSpa,
        .pokemonScale = 329,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VILEPLUME] =
    {
        .categoryName = _("Flower"),
        .categoryNameSpa = _("Flor"),
        .height = 12,
        .weight = 186,
        .description = gVileplumePokedexText,
        .descriptionSpa = gVileplumePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 4,
        .trainerScale = 272,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PARAS] =
    {
        .categoryName = _("Mushroom"),
        .categoryNameSpa = _("Hongo"),
        .height = 3,
        .weight = 54,
        .description = gParasPokedexText,
        .descriptionSpa = gParasPokedexTextSpa,
        .pokemonScale = 546,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PARASECT] =
    {
        .categoryName = _("Mushroom"),
        .categoryNameSpa = _("Hongo"),
        .height = 10,
        .weight = 295,
        .description = gParasectPokedexText,
        .descriptionSpa = gParasectPokedexTextSpa,
        .pokemonScale = 307,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VENONAT] =
    {
        .categoryName = _("Insect"),
        .categoryNameSpa = _("Insecto"),
        .height = 10,
        .weight = 300,
        .description = gVenonatPokedexText,
        .descriptionSpa = gVenonatPokedexTextSpa,
        .pokemonScale = 360,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VENOMOTH] =
    {
        .categoryName = _("Poison Moth"),
        .categoryNameSpa = _("Polilla V."),
        .height = 15,
        .weight = 125,
        .description = gVenomothPokedexText,
        .descriptionSpa = gVenomothPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 293,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_DIGLETT] =
    {
        .categoryName = _("Mole"),
        .categoryNameSpa = _("Topo"),
        .height = 2,
        .weight = 8,
        .description = gDiglettPokedexText,
        .descriptionSpa = gDiglettPokedexTextSpa,
        .pokemonScale = 706,
        .pokemonOffset = 22,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DUGTRIO] =
    {
        .categoryName = _("Mole"),
        .categoryNameSpa = _("Topo"),
        .height = 7,
        .weight = 333,
        .description = gDugtrioPokedexText,
        .descriptionSpa = gDugtrioPokedexTextSpa,
        .pokemonScale = 384,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MEOWTH] =
    {
        .categoryName = _("Scratch Cat"),
        .categoryNameSpa = _("Gato Araña"),
        .height = 4,
        .weight = 42,
        .description = gMeowthPokedexText,
        .descriptionSpa = gMeowthPokedexTextSpa,
        .pokemonScale = 480,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PERSIAN] =
    {
        .categoryName = _("Classy Cat"),
        .categoryNameSpa = _("Gato Fino"),
        .height = 10,
        .weight = 320,
        .description = gPersianPokedexText,
        .descriptionSpa = gPersianPokedexTextSpa,
        .pokemonScale = 320,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PSYDUCK] =
    {
        .categoryName = _("Duck"),
        .categoryNameSpa = _("Pato"),
        .height = 8,
        .weight = 196,
        .description = gPsyduckPokedexText,
        .descriptionSpa = gPsyduckPokedexTextSpa,
        .pokemonScale = 347,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GOLDUCK] =
    {
        .categoryName = _("Duck"),
        .categoryNameSpa = _("Pato"),
        .height = 17,
        .weight = 766,
        .description = gGolduckPokedexText,
        .descriptionSpa = gGolduckPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 4,
        .trainerScale = 287,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_MANKEY] =
    {
        .categoryName = _("Pig Monkey"),
        .categoryNameSpa = _("Mono Cerdo"),
        .height = 5,
        .weight = 280,
        .description = gMankeyPokedexText,
        .descriptionSpa = gMankeyPokedexTextSpa,
        .pokemonScale = 388,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PRIMEAPE] =
    {
        .categoryName = _("Pig Monkey"),
        .categoryNameSpa = _("Mono Cerdo"),
        .height = 10,
        .weight = 320,
        .description = gPrimeapePokedexText,
        .descriptionSpa = gPrimeapePokedexTextSpa,
        .pokemonScale = 326,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GROWLITHE] =
    {
        .categoryName = _("Puppy"),
        .categoryNameSpa = _("Perrito"),
        .height = 7,
        .weight = 190,
        .description = gGrowlithePokedexText,
        .descriptionSpa = gGrowlithePokedexTextSpa,
        .pokemonScale = 346,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ARCANINE] =
    {
        .categoryName = _("Legendary"),
        .categoryNameSpa = _("Legendario"),
        .height = 19,
        .weight = 1550,
        .description = gArcaninePokedexText,
        .descriptionSpa = gArcaninePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = -1,
        .trainerScale = 312,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_POLIWAG] =
    {
        .categoryName = _("Tadpole"),
        .categoryNameSpa = _("Renacuajo"),
        .height = 6,
        .weight = 124,
        .description = gPoliwagPokedexText,
        .descriptionSpa = gPoliwagPokedexTextSpa,
        .pokemonScale = 353,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_POLIWHIRL] =
    {
        .categoryName = _("Tadpole"),
        .categoryNameSpa = _("Renacuajo"),
        .height = 10,
        .weight = 200,
        .description = gPoliwhirlPokedexText,
        .descriptionSpa = gPoliwhirlPokedexTextSpa,
        .pokemonScale = 288,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_POLIWRATH] =
    {
        .categoryName = _("Tadpole"),
        .categoryNameSpa = _("Renacuajo"),
        .height = 13,
        .weight = 540,
        .description = gPoliwrathPokedexText,
        .descriptionSpa = gPoliwrathPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ABRA] =
    {
        .categoryName = _("PSI"),
        .categoryNameSpa = _("PSI"),
        .height = 9,
        .weight = 195,
        .description = gAbraPokedexText,
        .descriptionSpa = gAbraPokedexTextSpa,
        .pokemonScale = 374,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KADABRA] =
    {
        .categoryName = _("PSI"),
        .categoryNameSpa = _("PSI"),
        .height = 13,
        .weight = 565,
        .description = gKadabraPokedexText,
        .descriptionSpa = gKadabraPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ALAKAZAM] =
    {
        .categoryName = _("PSI"),
        .categoryNameSpa = _("PSI"),
        .height = 15,
        .weight = 480,
        .description = gAlakazamPokedexText,
        .descriptionSpa = gAlakazamPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = -1,
        .trainerScale = 271,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_MACHOP] =
    {
        .categoryName = _("Superpower"),
        .categoryNameSpa = _("Superpoder"),
        .height = 8,
        .weight = 195,
        .description = gMachopPokedexText,
        .descriptionSpa = gMachopPokedexTextSpa,
        .pokemonScale = 320,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MACHOKE] =
    {
        .categoryName = _("Superpower"),
        .categoryNameSpa = _("Superpoder"),
        .height = 15,
        .weight = 705,
        .description = gMachokePokedexText,
        .descriptionSpa = gMachokePokedexTextSpa,
        .pokemonScale = 304,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MACHAMP] =
    {
        .categoryName = _("Superpower"),
        .categoryNameSpa = _("Superpoder"),
        .height = 16,
        .weight = 1300,
        .description = gMachampPokedexText,
        .descriptionSpa = gMachampPokedexTextSpa,
        .pokemonScale = 278,
        .pokemonOffset = 2,
        .trainerScale = 283,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BELLSPROUT] =
    {
        .categoryName = _("Flower"),
        .categoryNameSpa = _("Flor"),
        .height = 7,
        .weight = 40,
        .description = gBellsproutPokedexText,
        .descriptionSpa = gBellsproutPokedexTextSpa,
        .pokemonScale = 354,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WEEPINBELL] =
    {
        .categoryName = _("Flycatcher"),
        .categoryNameSpa = _("Matamoscas"),
        .height = 10,
        .weight = 64,
        .description = gWeepinbellPokedexText,
        .descriptionSpa = gWeepinbellPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VICTREEBEL] =
    {
        .categoryName = _("Flycatcher"),
        .categoryNameSpa = _("Matamoscas"),
        .height = 17,
        .weight = 155,
        .description = gVictreebelPokedexText,
        .descriptionSpa = gVictreebelPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 302,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_TENTACOOL] =
    {
        .categoryName = _("Jellyfish"),
        .categoryNameSpa = _("Medusa"),
        .height = 9,
        .weight = 455,
        .description = gTentacoolPokedexText,
        .descriptionSpa = gTentacoolPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TENTACRUEL] =
    {
        .categoryName = _("Jellyfish"),
        .categoryNameSpa = _("Medusa"),
        .height = 16,
        .weight = 550,
        .description = gTentacruelPokedexText,
        .descriptionSpa = gTentacruelPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = -1,
        .trainerScale = 312,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_GEODUDE] =
    {
        .categoryName = _("Rock"),
        .categoryNameSpa = _("Roca"),
        .height = 4,
        .weight = 200,
        .description = gGeodudePokedexText,
        .descriptionSpa = gGeodudePokedexTextSpa,
        .pokemonScale = 330,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GRAVELER] =
    {
        .categoryName = _("Rock"),
        .categoryNameSpa = _("Roca"),
        .height = 10,
        .weight = 1050,
        .description = gGravelerPokedexText,
        .descriptionSpa = gGravelerPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 8,
        .trainerScale = 305,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_GOLEM] =
    {
        .categoryName = _("Megaton"),
        .categoryNameSpa = _("Megatón"),
        .height = 14,
        .weight = 3000,
        .description = gGolemPokedexText,
        .descriptionSpa = gGolemPokedexTextSpa,
        .pokemonScale = 266,
        .pokemonOffset = 3,
        .trainerScale = 298,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_PONYTA] =
    {
        .categoryName = _("Fire Horse"),
        .categoryNameSpa = _("Caballo F."),
        .height = 10,
        .weight = 300,
        .description = gPonytaPokedexText,
        .descriptionSpa = gPonytaPokedexTextSpa,
        .pokemonScale = 288,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RAPIDASH] =
    {
        .categoryName = _("Fire Horse"),
        .categoryNameSpa = _("Caballo F."),
        .height = 17,
        .weight = 950,
        .description = gRapidashPokedexText,
        .descriptionSpa = gRapidashPokedexTextSpa,
        .pokemonScale = 282,
        .pokemonOffset = -1,
        .trainerScale = 312,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_SLOWPOKE] =
    {
        .categoryName = _("Dopey"),
        .categoryNameSpa = _("Atontado"),
        .height = 12,
        .weight = 360,
        .description = gSlowpokePokedexText,
        .descriptionSpa = gSlowpokePokedexTextSpa,
        .pokemonScale = 271,
        .pokemonOffset = 10,
        .trainerScale = 272,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SLOWBRO] =
    {
        .categoryName = _("Hermit Crab"),
        .categoryNameSpa = _("Ermitaño"),
        .height = 16,
        .weight = 785,
        .description = gSlowbroPokedexText,
        .descriptionSpa = gSlowbroPokedexTextSpa,
        .pokemonScale = 257,
        .pokemonOffset = -2,
        .trainerScale = 312,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_MAGNEMITE] =
    {
        .categoryName = _("Magnet"),
        .categoryNameSpa = _("Imán"),
        .height = 3,
        .weight = 60,
        .description = gMagnemitePokedexText,
        .descriptionSpa = gMagnemitePokedexTextSpa,
        .pokemonScale = 294,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAGNETON] =
    {
        .categoryName = _("Magnet"),
        .categoryNameSpa = _("Imán"),
        .height = 10,
        .weight = 600,
        .description = gMagnetonPokedexText,
        .descriptionSpa = gMagnetonPokedexTextSpa,
        .pokemonScale = 293,
        .pokemonOffset = -4,
        .trainerScale = 273,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FARFETCHD] =
    {
        .categoryName = _("Wild Duck"),
        .categoryNameSpa = _("Pato Salv."),
        .height = 8,
        .weight = 150,
        .description = gFarfetchdPokedexText,
        .descriptionSpa = gFarfetchdPokedexTextSpa,
        .pokemonScale = 317,
        .pokemonOffset = -2,
        .trainerScale = 256,
        .trainerOffset = -3,
    },

    [NATIONAL_DEX_DODUO] =
    {
        .categoryName = _("Twin Bird"),
        .categoryNameSpa = _("Ave Gemela"),
        .height = 14,
        .weight = 392,
        .description = gDoduoPokedexText,
        .descriptionSpa = gDoduoPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 287,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_DODRIO] =
    {
        .categoryName = _("Triple Bird"),
        .categoryNameSpa = _("Ave Triple"),
        .height = 18,
        .weight = 852,
        .description = gDodrioPokedexText,
        .descriptionSpa = gDodrioPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = -2,
        .trainerScale = 296,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_SEEL] =
    {
        .categoryName = _("Sea Lion"),
        .categoryNameSpa = _("León Mar."),
        .height = 11,
        .weight = 900,
        .description = gSeelPokedexText,
        .descriptionSpa = gSeelPokedexTextSpa,
        .pokemonScale = 298,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DEWGONG] =
    {
        .categoryName = _("Sea Lion"),
        .categoryNameSpa = _("León Mar."),
        .height = 17,
        .weight = 1200,
        .description = gDewgongPokedexText,
        .descriptionSpa = gDewgongPokedexTextSpa,
        .pokemonScale = 288,
        .pokemonOffset = 1,
        .trainerScale = 306,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_GRIMER] =
    {
        .categoryName = _("Sludge"),
        .categoryNameSpa = _("Lodo"),
        .height = 9,
        .weight = 300,
        .description = gGrimerPokedexText,
        .descriptionSpa = gGrimerPokedexTextSpa,
        .pokemonScale = 258,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MUK] =
    {
        .categoryName = _("Sludge"),
        .categoryNameSpa = _("Lodo"),
        .height = 12,
        .weight = 300,
        .description = gMukPokedexText,
        .descriptionSpa = gMukPokedexTextSpa,
        .pokemonScale = 288,
        .pokemonOffset = 7,
        .trainerScale = 288,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_SHELLDER] =
    {
        .categoryName = _("Bivalve"),
        .categoryNameSpa = _("Bivalvo"),
        .height = 3,
        .weight = 40,
        .description = gShellderPokedexText,
        .descriptionSpa = gShellderPokedexTextSpa,
        .pokemonScale = 643,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CLOYSTER] =
    {
        .categoryName = _("Bivalve"),
        .categoryNameSpa = _("Bivalvo"),
        .height = 15,
        .weight = 1325,
        .description = gCloysterPokedexText,
        .descriptionSpa = gCloysterPokedexTextSpa,
        .pokemonScale = 264,
        .pokemonOffset = 0,
        .trainerScale = 288,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_GASTLY] =
    {
        .categoryName = _("Gas"),
        .categoryNameSpa = _("Gas"),
        .height = 13,
        .weight = 1,
        .description = gGastlyPokedexText,
        .descriptionSpa = gGastlyPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HAUNTER] =
    {
        .categoryName = _("Gas"),
        .categoryNameSpa = _("Gas"),
        .height = 16,
        .weight = 1,
        .description = gHaunterPokedexText,
        .descriptionSpa = gHaunterPokedexTextSpa,
        .pokemonScale = 269,
        .pokemonOffset = 2,
        .trainerScale = 308,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_GENGAR] =
    {
        .categoryName = _("Shadow"),
        .categoryNameSpa = _("Sombra"),
        .height = 15,
        .weight = 405,
        .description = gGengarPokedexText,
        .descriptionSpa = gGengarPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 4,
        .trainerScale = 317,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_ONIX] =
    {
        .categoryName = _("Rock Snake"),
        .categoryNameSpa = _("Serp. Roca"),
        .height = 88,
        .weight = 2100,
        .description = gOnixPokedexText,
        .descriptionSpa = gOnixPokedexTextSpa,
        .pokemonScale = 257,
        .pokemonOffset = 0,
        .trainerScale = 515,
        .trainerOffset = 12,
    },

    [NATIONAL_DEX_DROWZEE] =
    {
        .categoryName = _("Hypnosis"),
        .categoryNameSpa = _("Hipnosis"),
        .height = 10,
        .weight = 324,
        .description = gDrowzeePokedexText,
        .descriptionSpa = gDrowzeePokedexTextSpa,
        .pokemonScale = 274,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HYPNO] =
    {
        .categoryName = _("Hypnosis"),
        .categoryNameSpa = _("Hipnosis"),
        .height = 16,
        .weight = 756,
        .description = gHypnoPokedexText,
        .descriptionSpa = gHypnoPokedexTextSpa,
        .pokemonScale = 298,
        .pokemonOffset = 3,
        .trainerScale = 310,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_KRABBY] =
    {
        .categoryName = _("River Crab"),
        .categoryNameSpa = _("Cangrejo"),
        .height = 4,
        .weight = 65,
        .description = gKrabbyPokedexText,
        .descriptionSpa = gKrabbyPokedexTextSpa,
        .pokemonScale = 469,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KINGLER] =
    {
        .categoryName = _("Pincer"),
        .categoryNameSpa = _("Tenaza"),
        .height = 13,
        .weight = 600,
        .description = gKinglerPokedexText,
        .descriptionSpa = gKinglerPokedexTextSpa,
        .pokemonScale = 287,
        .pokemonOffset = 3,
        .trainerScale = 308,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_VOLTORB] =
    {
        .categoryName = _("Ball"),
        .categoryNameSpa = _("Bola"),
        .height = 5,
        .weight = 104,
        .description = gVoltorbPokedexText,
        .descriptionSpa = gVoltorbPokedexTextSpa,
        .pokemonScale = 364,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ELECTRODE] =
    {
        .categoryName = _("Ball"),
        .categoryNameSpa = _("Bola"),
        .height = 12,
        .weight = 666,
        .description = gElectrodePokedexText,
        .descriptionSpa = gElectrodePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_EXEGGCUTE] =
    {
        .categoryName = _("Egg"),
        .categoryNameSpa = _("Huevo"),
        .height = 4,
        .weight = 25,
        .description = gExeggcutePokedexText,
        .descriptionSpa = gExeggcutePokedexTextSpa,
        .pokemonScale = 495,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_EXEGGUTOR] =
    {
        .categoryName = _("Coconut"),
        .categoryNameSpa = _("Coco"),
        .height = 20,
        .weight = 1200,
        .description = gExeggutorPokedexText,
        .descriptionSpa = gExeggutorPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 0,
        .trainerScale = 376,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_CUBONE] =
    {
        .categoryName = _("Lonely"),
        .categoryNameSpa = _("Solitario"),
        .height = 4,
        .weight = 65,
        .description = gCubonePokedexText,
        .descriptionSpa = gCubonePokedexTextSpa,
        .pokemonScale = 545,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAROWAK] =
    {
        .categoryName = _("Bone Keeper"),
        .categoryNameSpa = _("Apilahueso"),
        .height = 10,
        .weight = 450,
        .description = gMarowakPokedexText,
        .descriptionSpa = gMarowakPokedexTextSpa,
        .pokemonScale = 293,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HITMONLEE] =
    {
        .categoryName = _("Kicking"),
        .categoryNameSpa = _("Patada"),
        .height = 15,
        .weight = 498,
        .description = gHitmonleePokedexText,
        .descriptionSpa = gHitmonleePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 273,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HITMONCHAN] =
    {
        .categoryName = _("Punching"),
        .categoryNameSpa = _("Puñetazo"),
        .height = 14,
        .weight = 502,
        .description = gHitmonchanPokedexText,
        .descriptionSpa = gHitmonchanPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 264,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LICKITUNG] =
    {
        .categoryName = _("Licking"),
        .categoryNameSpa = _("Lametazo"),
        .height = 12,
        .weight = 655,
        .description = gLickitungPokedexText,
        .descriptionSpa = gLickitungPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 3,
        .trainerScale = 272,
        .trainerOffset = -3,
    },

    [NATIONAL_DEX_KOFFING] =
    {
        .categoryName = _("Poison Gas"),
        .categoryNameSpa = _("Gas Veneno"),
        .height = 6,
        .weight = 10,
        .description = gKoffingPokedexText,
        .descriptionSpa = gKoffingPokedexTextSpa,
        .pokemonScale = 369,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WEEZING] =
    {
        .categoryName = _("Poison Gas"),
        .categoryNameSpa = _("Gas Veneno"),
        .height = 12,
        .weight = 95,
        .description = gWeezingPokedexText,
        .descriptionSpa = gWeezingPokedexTextSpa,
        .pokemonScale = 321,
        .pokemonOffset = -1,
        .trainerScale = 276,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_RHYHORN] =
    {
        .categoryName = _("Spikes"),
        .categoryNameSpa = _("Clavos"),
        .height = 10,
        .weight = 1150,
        .description = gRhyhornPokedexText,
        .descriptionSpa = gRhyhornPokedexTextSpa,
        .pokemonScale = 291,
        .pokemonOffset = 7,
        .trainerScale = 276,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RHYDON] =
    {
        .categoryName = _("Drill"),
        .categoryNameSpa = _("Taladro"),
        .height = 19,
        .weight = 1200,
        .description = gRhydonPokedexText,
        .descriptionSpa = gRhydonPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = -1,
        .trainerScale = 344,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_CHANSEY] =
    {
        .categoryName = _("Egg"),
        .categoryNameSpa = _("Huevo"),
        .height = 11,
        .weight = 346,
        .description = gChanseyPokedexText,
        .descriptionSpa = gChanseyPokedexTextSpa,
        .pokemonScale = 257,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TANGELA] =
    {
        .categoryName = _("Vine"),
        .categoryNameSpa = _("Enredadera"),
        .height = 10,
        .weight = 350,
        .description = gTangelaPokedexText,
        .descriptionSpa = gTangelaPokedexTextSpa,
        .pokemonScale = 320,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KANGASKHAN] =
    {
        .categoryName = _("Parent"),
        .categoryNameSpa = _("Padres"),
        .height = 22,
        .weight = 800,
        .description = gKangaskhanPokedexText,
        .descriptionSpa = gKangaskhanPokedexTextSpa,
        .pokemonScale = 257,
        .pokemonOffset = -3,
        .trainerScale = 349,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_HORSEA] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 4,
        .weight = 80,
        .description = gHorseaPokedexText,
        .descriptionSpa = gHorseaPokedexTextSpa,
        .pokemonScale = 399,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SEADRA] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 12,
        .weight = 250,
        .description = gSeadraPokedexText,
        .descriptionSpa = gSeadraPokedexTextSpa,
        .pokemonScale = 296,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GOLDEEN] =
    {
        .categoryName = _("Goldfish"),
        .categoryNameSpa = _("Pez Color"),
        .height = 6,
        .weight = 150,
        .description = gGoldeenPokedexText,
        .descriptionSpa = gGoldeenPokedexTextSpa,
        .pokemonScale = 379,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SEAKING] =
    {
        .categoryName = _("Goldfish"),
        .categoryNameSpa = _("Pez Color"),
        .height = 13,
        .weight = 390,
        .description = gSeakingPokedexText,
        .descriptionSpa = gSeakingPokedexTextSpa,
        .pokemonScale = 304,
        .pokemonOffset = 1,
        .trainerScale = 288,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_STARYU] =
    {
        .categoryName = _("Star Shape"),
        .categoryNameSpa = _("Estrellada"),
        .height = 8,
        .weight = 345,
        .description = gStaryuPokedexText,
        .descriptionSpa = gStaryuPokedexTextSpa,
        .pokemonScale = 326,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_STARMIE] =
    {
        .categoryName = _("Mysterious"),
        .categoryNameSpa = _("Misterioso"),
        .height = 11,
        .weight = 800,
        .description = gStarmiePokedexText,
        .descriptionSpa = gStarmiePokedexTextSpa,
        .pokemonScale = 301,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MR_MIME] =
    {
        .categoryName = _("Barrier"),
        .categoryNameSpa = _("Barrera"),
        .height = 13,
        .weight = 545,
        .description = gMrmimePokedexText,
        .descriptionSpa = gMrmimePokedexTextSpa,
        .pokemonScale = 258,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SCYTHER] =
    {
        .categoryName = _("Mantis"),
        .categoryNameSpa = _("Mantis"),
        .height = 15,
        .weight = 560,
        .description = gScytherPokedexText,
        .descriptionSpa = gScytherPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 0,
        .trainerScale = 293,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_JYNX] =
    {
        .categoryName = _("Human Shape"),
        .categoryNameSpa = _("F. Humana"),
        .height = 14,
        .weight = 406,
        .description = gJynxPokedexText,
        .descriptionSpa = gJynxPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 300,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_ELECTABUZZ] =
    {
        .categoryName = _("Electric"),
        .categoryNameSpa = _("Eléctrico"),
        .height = 11,
        .weight = 300,
        .description = gElectabuzzPokedexText,
        .descriptionSpa = gElectabuzzPokedexTextSpa,
        .pokemonScale = 330,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAGMAR] =
    {
        .categoryName = _("Spitfire"),
        .categoryNameSpa = _("Escupfuego"),
        .height = 13,
        .weight = 445,
        .description = gMagmarPokedexText,
        .descriptionSpa = gMagmarPokedexTextSpa,
        .pokemonScale = 293,
        .pokemonOffset = 4,
        .trainerScale = 272,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PINSIR] =
    {
        .categoryName = _("Stag Beetle"),
        .categoryNameSpa = _("Escarabajo"),
        .height = 15,
        .weight = 550,
        .description = gPinsirPokedexText,
        .descriptionSpa = gPinsirPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 257,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TAUROS] =
    {
        .categoryName = _("Wild Bull"),
        .categoryNameSpa = _("Toro Bravo"),
        .height = 14,
        .weight = 884,
        .description = gTaurosPokedexText,
        .descriptionSpa = gTaurosPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 312,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_MAGIKARP] =
    {
        .categoryName = _("Fish"),
        .categoryNameSpa = _("Pez"),
        .height = 9,
        .weight = 100,
        .description = gMagikarpPokedexText,
        .descriptionSpa = gMagikarpPokedexTextSpa,
        .pokemonScale = 317,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GYARADOS] =
    {
        .categoryName = _("Atrocious"),
        .categoryNameSpa = _("Atrocidad"),
        .height = 65,
        .weight = 2350,
        .description = gGyaradosPokedexText,
        .descriptionSpa = gGyaradosPokedexTextSpa,
        .pokemonScale = 288,
        .pokemonOffset = -1,
        .trainerScale = 512,
        .trainerOffset = 11,
    },

    [NATIONAL_DEX_LAPRAS] =
    {
        .categoryName = _("Transport"),
        .categoryNameSpa = _("Transporte"),
        .height = 25,
        .weight = 2200,
        .description = gLaprasPokedexText,
        .descriptionSpa = gLaprasPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 425,
        .trainerOffset = 8,
    },

    [NATIONAL_DEX_DITTO] =
    {
        .categoryName = _("Transform"),
        .categoryNameSpa = _("Transform."),
        .height = 3,
        .weight = 40,
        .description = gDittoPokedexText,
        .descriptionSpa = gDittoPokedexTextSpa,
        .pokemonScale = 602,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_EEVEE] =
    {
        .categoryName = _("Evolution"),
        .categoryNameSpa = _("Evolución"),
        .height = 3,
        .weight = 65,
        .description = gEeveePokedexText,
        .descriptionSpa = gEeveePokedexTextSpa,
        .pokemonScale = 476,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VAPOREON] =
    {
        .categoryName = _("Bubble Jet"),
        .categoryNameSpa = _("Burbuja"),
        .height = 10,
        .weight = 290,
        .description = gVaporeonPokedexText,
        .descriptionSpa = gVaporeonPokedexTextSpa,
        .pokemonScale = 316,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_JOLTEON] =
    {
        .categoryName = _("Lightning"),
        .categoryNameSpa = _("Relámpago"),
        .height = 8,
        .weight = 245,
        .description = gJolteonPokedexText,
        .descriptionSpa = gJolteonPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FLAREON] =
    {
        .categoryName = _("Flame"),
        .categoryNameSpa = _("Llama"),
        .height = 9,
        .weight = 250,
        .description = gFlareonPokedexText,
        .descriptionSpa = gFlareonPokedexTextSpa,
        .pokemonScale = 302,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PORYGON] =
    {
        .categoryName = _("Virtual"),
        .categoryNameSpa = _("Virtual"),
        .height = 8,
        .weight = 365,
        .description = gPorygonPokedexText,
        .descriptionSpa = gPorygonPokedexTextSpa,
        .pokemonScale = 328,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_OMANYTE] =
    {
        .categoryName = _("Spiral"),
        .categoryNameSpa = _("Espiral"),
        .height = 4,
        .weight = 75,
        .description = gOmanytePokedexText,
        .descriptionSpa = gOmanytePokedexTextSpa,
        .pokemonScale = 521,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_OMASTAR] =
    {
        .categoryName = _("Spiral"),
        .categoryNameSpa = _("Espiral"),
        .height = 10,
        .weight = 350,
        .description = gOmastarPokedexText,
        .descriptionSpa = gOmastarPokedexTextSpa,
        .pokemonScale = 307,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KABUTO] =
    {
        .categoryName = _("Shellfish"),
        .categoryNameSpa = _("Marisco"),
        .height = 5,
        .weight = 115,
        .description = gKabutoPokedexText,
        .descriptionSpa = gKabutoPokedexTextSpa,
        .pokemonScale = 438,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KABUTOPS] =
    {
        .categoryName = _("Shellfish"),
        .categoryNameSpa = _("Marisco"),
        .height = 13,
        .weight = 405,
        .description = gKabutopsPokedexText,
        .descriptionSpa = gKabutopsPokedexTextSpa,
        .pokemonScale = 271,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_AERODACTYL] =
    {
        .categoryName = _("Fossil"),
        .categoryNameSpa = _("Fósil"),
        .height = 18,
        .weight = 590,
        .description = gAerodactylPokedexText,
        .descriptionSpa = gAerodactylPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = -1,
        .trainerScale = 317,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_SNORLAX] =
    {
        .categoryName = _("Sleeping"),
        .categoryNameSpa = _("Dormir"),
        .height = 21,
        .weight = 4600,
        .description = gSnorlaxPokedexText,
        .descriptionSpa = gSnorlaxPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 1,
        .trainerScale = 408,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_ARTICUNO] =
    {
        .categoryName = _("Freeze"),
        .categoryNameSpa = _("Congelar"),
        .height = 17,
        .weight = 554,
        .description = gArticunoPokedexText,
        .descriptionSpa = gArticunoPokedexTextSpa,
        .pokemonScale = 278,
        .pokemonOffset = 0,
        .trainerScale = 308,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_ZAPDOS] =
    {
        .categoryName = _("Electric"),
        .categoryNameSpa = _("Eléctrico"),
        .height = 16,
        .weight = 526,
        .description = gZapdosPokedexText,
        .descriptionSpa = gZapdosPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 1,
        .trainerScale = 330,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_MOLTRES] =
    {
        .categoryName = _("Flame"),
        .categoryNameSpa = _("Llama"),
        .height = 20,
        .weight = 600,
        .description = gMoltresPokedexText,
        .descriptionSpa = gMoltresPokedexTextSpa,
        .pokemonScale = 270,
        .pokemonOffset = 1,
        .trainerScale = 379,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_DRATINI] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 18,
        .weight = 33,
        .description = gDratiniPokedexText,
        .descriptionSpa = gDratiniPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 8,
        .trainerScale = 386,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_DRAGONAIR] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 40,
        .weight = 165,
        .description = gDragonairPokedexText,
        .descriptionSpa = gDragonairPokedexTextSpa,
        .pokemonScale = 274,
        .pokemonOffset = 0,
        .trainerScale = 423,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_DRAGONITE] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 22,
        .weight = 2100,
        .description = gDragonitePokedexText,
        .descriptionSpa = gDragonitePokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = -1,
        .trainerScale = 342,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_MEWTWO] =
    {
        .categoryName = _("Genetic"),
        .categoryNameSpa = _("Genético"),
        .height = 20,
        .weight = 1220,
        .description = gMewtwoPokedexText,
        .descriptionSpa = gMewtwoPokedexTextSpa,
        .pokemonScale = 276,
        .pokemonOffset = -1,
        .trainerScale = 342,
        .trainerOffset = 5,
    },

    [NATIONAL_DEX_MEW] =
    {
        .categoryName = _("New Species"),
        .categoryNameSpa = _("N. Especie"),
        .height = 4,
        .weight = 40,
        .description = gMewPokedexText,
        .descriptionSpa = gMewPokedexTextSpa,
        .pokemonScale = 460,
        .pokemonOffset = -2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CHIKORITA] =
    {
        .categoryName = _("Leaf"),
        .categoryNameSpa = _("Hoja"),
        .height = 9,
        .weight = 64,
        .description = gChikoritaPokedexText,
        .descriptionSpa = gChikoritaPokedexTextSpa,
        .pokemonScale = 512,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BAYLEEF] =
    {
        .categoryName = _("Leaf"),
        .categoryNameSpa = _("Hoja"),
        .height = 12,
        .weight = 158,
        .description = gBayleefPokedexText,
        .descriptionSpa = gBayleefPokedexTextSpa,
        .pokemonScale = 296,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MEGANIUM] =
    {
        .categoryName = _("Herb"),
        .categoryNameSpa = _("Hierba"),
        .height = 18,
        .weight = 1005,
        .description = gMeganiumPokedexText,
        .descriptionSpa = gMeganiumPokedexTextSpa,
        .pokemonScale = 286,
        .pokemonOffset = 0,
        .trainerScale = 317,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_CYNDAQUIL] =
    {
        .categoryName = _("Fire Mouse"),
        .categoryNameSpa = _("Ratónfuego"),
        .height = 5,
        .weight = 79,
        .description = gCyndaquilPokedexText,
        .descriptionSpa = gCyndaquilPokedexTextSpa,
        .pokemonScale = 539,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_QUILAVA] =
    {
        .categoryName = _("Volcano"),
        .categoryNameSpa = _("Volcán"),
        .height = 9,
        .weight = 190,
        .description = gQuilavaPokedexText,
        .descriptionSpa = gQuilavaPokedexTextSpa,
        .pokemonScale = 329,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TYPHLOSION] =
    {
        .categoryName = _("Volcano"),
        .categoryNameSpa = _("Volcán"),
        .height = 17,
        .weight = 795,
        .description = gTyphlosionPokedexText,
        .descriptionSpa = gTyphlosionPokedexTextSpa,
        .pokemonScale = 284,
        .pokemonOffset = -1,
        .trainerScale = 287,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_TOTODILE] =
    {
        .categoryName = _("Big Jaw"),
        .categoryNameSpa = _("Fauces"),
        .height = 6,
        .weight = 95,
        .description = gTotodilePokedexText,
        .descriptionSpa = gTotodilePokedexTextSpa,
        .pokemonScale = 487,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CROCONAW] =
    {
        .categoryName = _("Big Jaw"),
        .categoryNameSpa = _("Fauces"),
        .height = 11,
        .weight = 250,
        .description = gCroconawPokedexText,
        .descriptionSpa = gCroconawPokedexTextSpa,
        .pokemonScale = 378,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FERALIGATR] =
    {
        .categoryName = _("Big Jaw"),
        .categoryNameSpa = _("Fauces"),
        .height = 23,
        .weight = 888,
        .description = gFeraligatrPokedexText,
        .descriptionSpa = gFeraligatrPokedexTextSpa,
        .pokemonScale = 282,
        .pokemonOffset = -1,
        .trainerScale = 375,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_SENTRET] =
    {
        .categoryName = _("Scout"),
        .categoryNameSpa = _("Explorador"),
        .height = 8,
        .weight = 60,
        .description = gSentretPokedexText,
        .descriptionSpa = gSentretPokedexTextSpa,
        .pokemonScale = 439,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FURRET] =
    {
        .categoryName = _("Long Body"),
        .categoryNameSpa = _("Alargado"),
        .height = 18,
        .weight = 325,
        .description = gFurretPokedexText,
        .descriptionSpa = gFurretPokedexTextSpa,
        .pokemonScale = 346,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HOOTHOOT] =
    {
        .categoryName = _("Owl"),
        .categoryNameSpa = _("Búho"),
        .height = 7,
        .weight = 212,
        .description = gHoothootPokedexText,
        .descriptionSpa = gHoothootPokedexTextSpa,
        .pokemonScale = 380,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NOCTOWL] =
    {
        .categoryName = _("Owl"),
        .categoryNameSpa = _("Búho"),
        .height = 16,
        .weight = 408,
        .description = gNoctowlPokedexText,
        .descriptionSpa = gNoctowlPokedexTextSpa,
        .pokemonScale = 278,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LEDYBA] =
    {
        .categoryName = _("Five Star"),
        .categoryNameSpa = _("5Estrellas"),
        .height = 10,
        .weight = 108,
        .description = gLedybaPokedexText,
        .descriptionSpa = gLedybaPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LEDIAN] =
    {
        .categoryName = _("Five Star"),
        .categoryNameSpa = _("5Estrellas"),
        .height = 14,
        .weight = 356,
        .description = gLedianPokedexText,
        .descriptionSpa = gLedianPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SPINARAK] =
    {
        .categoryName = _("String Spit"),
        .categoryNameSpa = _("Escupesoga"),
        .height = 5,
        .weight = 85,
        .description = gSpinarakPokedexText,
        .descriptionSpa = gSpinarakPokedexTextSpa,
        .pokemonScale = 414,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ARIADOS] =
    {
        .categoryName = _("Long Leg"),
        .categoryNameSpa = _("Pata Larga"),
        .height = 11,
        .weight = 335,
        .description = gAriadosPokedexText,
        .descriptionSpa = gAriadosPokedexTextSpa,
        .pokemonScale = 316,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CROBAT] =
    {
        .categoryName = _("Bat"),
        .categoryNameSpa = _("Murciélago"),
        .height = 18,
        .weight = 750,
        .description = gCrobatPokedexText,
        .descriptionSpa = gCrobatPokedexTextSpa,
        .pokemonScale = 279,
        .pokemonOffset = -1,
        .trainerScale = 313,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_CHINCHOU] =
    {
        .categoryName = _("Angler"),
        .categoryNameSpa = _("Rape"),
        .height = 5,
        .weight = 120,
        .description = gChinchouPokedexText,
        .descriptionSpa = gChinchouPokedexTextSpa,
        .pokemonScale = 424,
        .pokemonOffset = -2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LANTURN] =
    {
        .categoryName = _("Light"),
        .categoryNameSpa = _("Luz"),
        .height = 12,
        .weight = 225,
        .description = gLanturnPokedexText,
        .descriptionSpa = gLanturnPokedexTextSpa,
        .pokemonScale = 269,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PICHU] =
    {
        .categoryName = _("Tiny Mouse"),
        .categoryNameSpa = _("Ratoncito"),
        .height = 3,
        .weight = 20,
        .description = gPichuPokedexText,
        .descriptionSpa = gPichuPokedexTextSpa,
        .pokemonScale = 508,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CLEFFA] =
    {
        .categoryName = _("Star Shape"),
        .categoryNameSpa = _("Estrellada"),
        .height = 3,
        .weight = 30,
        .description = gCleffaPokedexText,
        .descriptionSpa = gCleffaPokedexTextSpa,
        .pokemonScale = 462,
        .pokemonOffset = 22,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_IGGLYBUFF] =
    {
        .categoryName = _("Balloon"),
        .categoryNameSpa = _("Globo"),
        .height = 3,
        .weight = 10,
        .description = gIgglybuffPokedexText,
        .descriptionSpa = gIgglybuffPokedexTextSpa,
        .pokemonScale = 457,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TOGEPI] =
    {
        .categoryName = _("Spike Ball"),
        .categoryNameSpa = _("Bolaclavos"),
        .height = 3,
        .weight = 15,
        .description = gTogepiPokedexText,
        .descriptionSpa = gTogepiPokedexTextSpa,
        .pokemonScale = 507,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TOGETIC] =
    {
        .categoryName = _("Happiness"),
        .categoryNameSpa = _("Felicidad"),
        .height = 6,
        .weight = 32,
        .description = gTogeticPokedexText,
        .descriptionSpa = gTogeticPokedexTextSpa,
        .pokemonScale = 424,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NATU] =
    {
        .categoryName = _("Tiny Bird"),
        .categoryNameSpa = _("Pajarito"),
        .height = 2,
        .weight = 20,
        .description = gNatuPokedexText,
        .descriptionSpa = gNatuPokedexTextSpa,
        .pokemonScale = 610,
        .pokemonOffset = 23,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_XATU] =
    {
        .categoryName = _("Mystic"),
        .categoryNameSpa = _("Místico"),
        .height = 15,
        .weight = 150,
        .description = gXatuPokedexText,
        .descriptionSpa = gXatuPokedexTextSpa,
        .pokemonScale = 258,
        .pokemonOffset = 4,
        .trainerScale = 317,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_MAREEP] =
    {
        .categoryName = _("Wool"),
        .categoryNameSpa = _("Lana"),
        .height = 6,
        .weight = 78,
        .description = gMareepPokedexText,
        .descriptionSpa = gMareepPokedexTextSpa,
        .pokemonScale = 379,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FLAAFFY] =
    {
        .categoryName = _("Wool"),
        .categoryNameSpa = _("Lana"),
        .height = 8,
        .weight = 133,
        .description = gFlaaffyPokedexText,
        .descriptionSpa = gFlaaffyPokedexTextSpa,
        .pokemonScale = 372,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_AMPHAROS] =
    {
        .categoryName = _("Light"),
        .categoryNameSpa = _("Luz"),
        .height = 14,
        .weight = 615,
        .description = gAmpharosPokedexText,
        .descriptionSpa = gAmpharosPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 2,
        .trainerScale = 283,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_BELLOSSOM] =
    {
        .categoryName = _("Flower"),
        .categoryNameSpa = _("Flor"),
        .height = 4,
        .weight = 58,
        .description = gBellossomPokedexText,
        .descriptionSpa = gBellossomPokedexTextSpa,
        .pokemonScale = 472,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MARILL] =
    {
        .categoryName = _("Aqua Mouse"),
        .categoryNameSpa = _("Ratón Agua"),
        .height = 4,
        .weight = 85,
        .description = gMarillPokedexText,
        .descriptionSpa = gMarillPokedexTextSpa,
        .pokemonScale = 476,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_AZUMARILL] =
    {
        .categoryName = _("Aqua Rabbit"),
        .categoryNameSpa = _("Conejoagua"),
        .height = 8,
        .weight = 285,
        .description = gAzumarillPokedexText,
        .descriptionSpa = gAzumarillPokedexTextSpa,
        .pokemonScale = 448,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SUDOWOODO] =
    {
        .categoryName = _("Imitation"),
        .categoryNameSpa = _("Imitación"),
        .height = 12,
        .weight = 380,
        .description = gSudowoodoPokedexText,
        .descriptionSpa = gSudowoodoPokedexTextSpa,
        .pokemonScale = 305,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_POLITOED] =
    {
        .categoryName = _("Frog"),
        .categoryNameSpa = _("Rana"),
        .height = 11,
        .weight = 339,
        .description = gPolitoedPokedexText,
        .descriptionSpa = gPolitoedPokedexTextSpa,
        .pokemonScale = 289,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HOPPIP] =
    {
        .categoryName = _("Cottonweed"),
        .categoryNameSpa = _("Algadón"),
        .height = 4,
        .weight = 5,
        .description = gHoppipPokedexText,
        .descriptionSpa = gHoppipPokedexTextSpa,
        .pokemonScale = 562,
        .pokemonOffset = -7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SKIPLOOM] =
    {
        .categoryName = _("Cottonweed"),
        .categoryNameSpa = _("Algadón"),
        .height = 6,
        .weight = 10,
        .description = gSkiploomPokedexText,
        .descriptionSpa = gSkiploomPokedexTextSpa,
        .pokemonScale = 387,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_JUMPLUFF] =
    {
        .categoryName = _("Cottonweed"),
        .categoryNameSpa = _("Algadón"),
        .height = 8,
        .weight = 30,
        .description = gJumpluffPokedexText,
        .descriptionSpa = gJumpluffPokedexTextSpa,
        .pokemonScale = 418,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_AIPOM] =
    {
        .categoryName = _("Long Tail"),
        .categoryNameSpa = _("Cola Larga"),
        .height = 8,
        .weight = 115,
        .description = gAipomPokedexText,
        .descriptionSpa = gAipomPokedexTextSpa,
        .pokemonScale = 363,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SUNKERN] =
    {
        .categoryName = _("Seed"),
        .categoryNameSpa = _("Semilla"),
        .height = 3,
        .weight = 18,
        .description = gSunkernPokedexText,
        .descriptionSpa = gSunkernPokedexTextSpa,
        .pokemonScale = 541,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SUNFLORA] =
    {
        .categoryName = _("Sun"),
        .categoryNameSpa = _("Sol"),
        .height = 8,
        .weight = 85,
        .description = gSunfloraPokedexText,
        .descriptionSpa = gSunfloraPokedexTextSpa,
        .pokemonScale = 374,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_YANMA] =
    {
        .categoryName = _("Clear Wing"),
        .categoryNameSpa = _("Ala Clara"),
        .height = 12,
        .weight = 380,
        .description = gYanmaPokedexText,
        .descriptionSpa = gYanmaPokedexTextSpa,
        .pokemonScale = 274,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WOOPER] =
    {
        .categoryName = _("Water Fish"),
        .categoryNameSpa = _("Pez Agua"),
        .height = 4,
        .weight = 85,
        .description = gWooperPokedexText,
        .descriptionSpa = gWooperPokedexTextSpa,
        .pokemonScale = 479,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_QUAGSIRE] =
    {
        .categoryName = _("Water Fish"),
        .categoryNameSpa = _("Pez Agua"),
        .height = 14,
        .weight = 750,
        .description = gQuagsirePokedexText,
        .descriptionSpa = gQuagsirePokedexTextSpa,
        .pokemonScale = 273,
        .pokemonOffset = 4,
        .trainerScale = 273,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ESPEON] =
    {
        .categoryName = _("Sun"),
        .categoryNameSpa = _("Sol"),
        .height = 9,
        .weight = 265,
        .description = gEspeonPokedexText,
        .descriptionSpa = gEspeonPokedexTextSpa,
        .pokemonScale = 363,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_UMBREON] =
    {
        .categoryName = _("Moonlight"),
        .categoryNameSpa = _("Luz Lunar"),
        .height = 10,
        .weight = 270,
        .description = gUmbreonPokedexText,
        .descriptionSpa = gUmbreonPokedexTextSpa,
        .pokemonScale = 317,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MURKROW] =
    {
        .categoryName = _("Darkness"),
        .categoryNameSpa = _("Oscuridad"),
        .height = 5,
        .weight = 21,
        .description = gMurkrowPokedexText,
        .descriptionSpa = gMurkrowPokedexTextSpa,
        .pokemonScale = 401,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SLOWKING] =
    {
        .categoryName = _("Royal"),
        .categoryNameSpa = _("Regio"),
        .height = 20,
        .weight = 795,
        .description = gSlowkingPokedexText,
        .descriptionSpa = gSlowkingPokedexTextSpa,
        .pokemonScale = 265,
        .pokemonOffset = -1,
        .trainerScale = 330,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_MISDREAVUS] =
    {
        .categoryName = _("Screech"),
        .categoryNameSpa = _("Chirrido"),
        .height = 7,
        .weight = 10,
        .description = gMisdreavusPokedexText,
        .descriptionSpa = gMisdreavusPokedexTextSpa,
        .pokemonScale = 407,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_UNOWN] =
    {
        .categoryName = _("Symbol"),
        .categoryNameSpa = _("Símbolo"),
        .height = 5,
        .weight = 50,
        .description = gUnownPokedexText,
        .descriptionSpa = gUnownPokedexTextSpa,
        .pokemonScale = 411,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WOBBUFFET] =
    {
        .categoryName = _("Patient"),
        .categoryNameSpa = _("Paciente"),
        .height = 13,
        .weight = 285,
        .description = gWobbuffetPokedexText,
        .descriptionSpa = gWobbuffetPokedexTextSpa,
        .pokemonScale = 274,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GIRAFARIG] =
    {
        .categoryName = _("Long Neck"),
        .categoryNameSpa = _("Grancuello"),
        .height = 15,
        .weight = 415,
        .description = gGirafarigPokedexText,
        .descriptionSpa = gGirafarigPokedexTextSpa,
        .pokemonScale = 281,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PINECO] =
    {
        .categoryName = _("Bagworm"),
        .categoryNameSpa = _("Larva"),
        .height = 6,
        .weight = 72,
        .description = gPinecoPokedexText,
        .descriptionSpa = gPinecoPokedexTextSpa,
        .pokemonScale = 445,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FORRETRESS] =
    {
        .categoryName = _("Bagworm"),
        .categoryNameSpa = _("Larva"),
        .height = 12,
        .weight = 1258,
        .description = gForretressPokedexText,
        .descriptionSpa = gForretressPokedexTextSpa,
        .pokemonScale = 293,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DUNSPARCE] =
    {
        .categoryName = _("Land Snake"),
        .categoryNameSpa = _("Serptierra"),
        .height = 15,
        .weight = 140,
        .description = gDunsparcePokedexText,
        .descriptionSpa = gDunsparcePokedexTextSpa,
        .pokemonScale = 284,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GLIGAR] =
    {
        .categoryName = _("Flyscorpion"),
        .categoryNameSpa = _("Escorpiala"),
        .height = 11,
        .weight = 648,
        .description = gGligarPokedexText,
        .descriptionSpa = gGligarPokedexTextSpa,
        .pokemonScale = 350,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_STEELIX] =
    {
        .categoryName = _("Iron Snake"),
        .categoryNameSpa = _("Serpférrea"),
        .height = 92,
        .weight = 4000,
        .description = gSteelixPokedexText,
        .descriptionSpa = gSteelixPokedexTextSpa,
        .pokemonScale = 278,
        .pokemonOffset = -1,
        .trainerScale = 557,
        .trainerOffset = 13,
    },

    [NATIONAL_DEX_SNUBBULL] =
    {
        .categoryName = _("Fairy"),
        .categoryNameSpa = _("Hada"),
        .height = 6,
        .weight = 78,
        .description = gSnubbullPokedexText,
        .descriptionSpa = gSnubbullPokedexTextSpa,
        .pokemonScale = 465,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GRANBULL] =
    {
        .categoryName = _("Fairy"),
        .categoryNameSpa = _("Hada"),
        .height = 14,
        .weight = 487,
        .description = gGranbullPokedexText,
        .descriptionSpa = gGranbullPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_QWILFISH] =
    {
        .categoryName = _("Balloon"),
        .categoryNameSpa = _("Globo"),
        .height = 5,
        .weight = 39,
        .description = gQwilfishPokedexText,
        .descriptionSpa = gQwilfishPokedexTextSpa,
        .pokemonScale = 430,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SCIZOR] =
    {
        .categoryName = _("Pincer"),
        .categoryNameSpa = _("Tenaza"),
        .height = 18,
        .weight = 1180,
        .description = gScizorPokedexText,
        .descriptionSpa = gScizorPokedexTextSpa,
        .pokemonScale = 282,
        .pokemonOffset = 0,
        .trainerScale = 282,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SHUCKLE] =
    {
        .categoryName = _("Mold"),
        .categoryNameSpa = _("Moho"),
        .height = 6,
        .weight = 205,
        .description = gShucklePokedexText,
        .descriptionSpa = gShucklePokedexTextSpa,
        .pokemonScale = 485,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HERACROSS] =
    {
        .categoryName = _("Single Horn"),
        .categoryNameSpa = _("Unicornio"),
        .height = 15,
        .weight = 540,
        .description = gHeracrossPokedexText,
        .descriptionSpa = gHeracrossPokedexTextSpa,
        .pokemonScale = 285,
        .pokemonOffset = 0,
        .trainerScale = 283,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SNEASEL] =
    {
        .categoryName = _("Sharp Claw"),
        .categoryNameSpa = _("Garra Filo"),
        .height = 9,
        .weight = 280,
        .description = gSneaselPokedexText,
        .descriptionSpa = gSneaselPokedexTextSpa,
        .pokemonScale = 413,
        .pokemonOffset = -3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TEDDIURSA] =
    {
        .categoryName = _("Little Bear"),
        .categoryNameSpa = _("Osito"),
        .height = 6,
        .weight = 88,
        .description = gTeddiursaPokedexText,
        .descriptionSpa = gTeddiursaPokedexTextSpa,
        .pokemonScale = 455,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_URSARING] =
    {
        .categoryName = _("Hibernator"),
        .categoryNameSpa = _("Hibernante"),
        .height = 18,
        .weight = 1258,
        .description = gUrsaringPokedexText,
        .descriptionSpa = gUrsaringPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 0,
        .trainerScale = 280,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SLUGMA] =
    {
        .categoryName = _("Lava"),
        .categoryNameSpa = _("Lava"),
        .height = 7,
        .weight = 350,
        .description = gSlugmaPokedexText,
        .descriptionSpa = gSlugmaPokedexTextSpa,
        .pokemonScale = 329,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAGCARGO] =
    {
        .categoryName = _("Lava"),
        .categoryNameSpa = _("Lava"),
        .height = 8,
        .weight = 550,
        .description = gMagcargoPokedexText,
        .descriptionSpa = gMagcargoPokedexTextSpa,
        .pokemonScale = 332,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SWINUB] =
    {
        .categoryName = _("Pig"),
        .categoryNameSpa = _("Cerdo"),
        .height = 4,
        .weight = 65,
        .description = gSwinubPokedexText,
        .descriptionSpa = gSwinubPokedexTextSpa,
        .pokemonScale = 324,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PILOSWINE] =
    {
        .categoryName = _("Swine"),
        .categoryNameSpa = _("Puerco"),
        .height = 11,
        .weight = 558,
        .description = gPiloswinePokedexText,
        .descriptionSpa = gPiloswinePokedexTextSpa,
        .pokemonScale = 306,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CORSOLA] =
    {
        .categoryName = _("Coral"),
        .categoryNameSpa = _("Coral"),
        .height = 6,
        .weight = 50,
        .description = gCorsolaPokedexText,
        .descriptionSpa = gCorsolaPokedexTextSpa,
        .pokemonScale = 410,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_REMORAID] =
    {
        .categoryName = _("Jet"),
        .categoryNameSpa = _("Reactor"),
        .height = 6,
        .weight = 120,
        .description = gRemoraidPokedexText,
        .descriptionSpa = gRemoraidPokedexTextSpa,
        .pokemonScale = 316,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_OCTILLERY] =
    {
        .categoryName = _("Jet"),
        .categoryNameSpa = _("Reactor"),
        .height = 9,
        .weight = 285,
        .description = gOctilleryPokedexText,
        .descriptionSpa = gOctilleryPokedexTextSpa,
        .pokemonScale = 296,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DELIBIRD] =
    {
        .categoryName = _("Delivery"),
        .categoryNameSpa = _("Reparto"),
        .height = 9,
        .weight = 160,
        .description = gDelibirdPokedexText,
        .descriptionSpa = gDelibirdPokedexTextSpa,
        .pokemonScale = 293,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MANTINE] =
    {
        .categoryName = _("Kite"),
        .categoryNameSpa = _("Milano"),
        .height = 21,
        .weight = 2200,
        .description = gMantinePokedexText,
        .descriptionSpa = gMantinePokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 0,
        .trainerScale = 360,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_SKARMORY] =
    {
        .categoryName = _("Armor Bird"),
        .categoryNameSpa = _("Coraza Ave"),
        .height = 17,
        .weight = 505,
        .description = gSkarmoryPokedexText,
        .descriptionSpa = gSkarmoryPokedexTextSpa,
        .pokemonScale = 285,
        .pokemonOffset = 0,
        .trainerScale = 276,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_HOUNDOUR] =
    {
        .categoryName = _("Dark"),
        .categoryNameSpa = _("Siniestro"),
        .height = 6,
        .weight = 108,
        .description = gHoundourPokedexText,
        .descriptionSpa = gHoundourPokedexTextSpa,
        .pokemonScale = 393,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HOUNDOOM] =
    {
        .categoryName = _("Dark"),
        .categoryNameSpa = _("Siniestro"),
        .height = 14,
        .weight = 350,
        .description = gHoundoomPokedexText,
        .descriptionSpa = gHoundoomPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KINGDRA] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 18,
        .weight = 1520,
        .description = gKingdraPokedexText,
        .descriptionSpa = gKingdraPokedexTextSpa,
        .pokemonScale = 257,
        .pokemonOffset = 1,
        .trainerScale = 293,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_PHANPY] =
    {
        .categoryName = _("Long Nose"),
        .categoryNameSpa = _("Narizotas"),
        .height = 5,
        .weight = 335,
        .description = gPhanpyPokedexText,
        .descriptionSpa = gPhanpyPokedexTextSpa,
        .pokemonScale = 465,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DONPHAN] =
    {
        .categoryName = _("Armor"),
        .categoryNameSpa = _("Coraza"),
        .height = 11,
        .weight = 1200,
        .description = gDonphanPokedexText,
        .descriptionSpa = gDonphanPokedexTextSpa,
        .pokemonScale = 313,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PORYGON2] =
    {
        .categoryName = _("Virtual"),
        .categoryNameSpa = _("Virtual"),
        .height = 6,
        .weight = 325,
        .description = gPorygon2PokedexText,
        .descriptionSpa = gPorygon2PokedexTextSpa,
        .pokemonScale = 320,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_STANTLER] =
    {
        .categoryName = _("Big Horn"),
        .categoryNameSpa = _("Grancuerno"),
        .height = 14,
        .weight = 712,
        .description = gStantlerPokedexText,
        .descriptionSpa = gStantlerPokedexTextSpa,
        .pokemonScale = 277,
        .pokemonOffset = -1,
        .trainerScale = 277,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SMEARGLE] =
    {
        .categoryName = _("Painter"),
        .categoryNameSpa = _("Pintor"),
        .height = 12,
        .weight = 580,
        .description = gSmearglePokedexText,
        .descriptionSpa = gSmearglePokedexTextSpa,
        .pokemonScale = 287,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TYROGUE] =
    {
        .categoryName = _("Scuffle"),
        .categoryNameSpa = _("Peleón"),
        .height = 7,
        .weight = 210,
        .description = gTyroguePokedexText,
        .descriptionSpa = gTyroguePokedexTextSpa,
        .pokemonScale = 292,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HITMONTOP] =
    {
        .categoryName = _("Handstand"),
        .categoryNameSpa = _("Boca Abajo"),
        .height = 14,
        .weight = 480,
        .description = gHitmontopPokedexText,
        .descriptionSpa = gHitmontopPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 257,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SMOOCHUM] =
    {
        .categoryName = _("Kiss"),
        .categoryNameSpa = _("Beso"),
        .height = 4,
        .weight = 60,
        .description = gSmoochumPokedexText,
        .descriptionSpa = gSmoochumPokedexTextSpa,
        .pokemonScale = 440,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ELEKID] =
    {
        .categoryName = _("Electric"),
        .categoryNameSpa = _("Eléctrico"),
        .height = 6,
        .weight = 235,
        .description = gElekidPokedexText,
        .descriptionSpa = gElekidPokedexTextSpa,
        .pokemonScale = 363,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAGBY] =
    {
        .categoryName = _("Live Coal"),
        .categoryNameSpa = _("Ascuas"),
        .height = 7,
        .weight = 214,
        .description = gMagbyPokedexText,
        .descriptionSpa = gMagbyPokedexTextSpa,
        .pokemonScale = 284,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MILTANK] =
    {
        .categoryName = _("Milk Cow"),
        .categoryNameSpa = _("Lechera"),
        .height = 12,
        .weight = 755,
        .description = gMiltankPokedexText,
        .descriptionSpa = gMiltankPokedexTextSpa,
        .pokemonScale = 280,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BLISSEY] =
    {
        .categoryName = _("Happiness"),
        .categoryNameSpa = _("Felicidad"),
        .height = 15,
        .weight = 468,
        .description = gBlisseyPokedexText,
        .descriptionSpa = gBlisseyPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 310,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_RAIKOU] =
    {
        .categoryName = _("Thunder"),
        .categoryNameSpa = _("Trueno"),
        .height = 19,
        .weight = 1780,
        .description = gRaikouPokedexText,
        .descriptionSpa = gRaikouPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 0,
        .trainerScale = 359,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_ENTEI] =
    {
        .categoryName = _("Volcano"),
        .categoryNameSpa = _("Volcán"),
        .height = 21,
        .weight = 1980,
        .description = gEnteiPokedexText,
        .descriptionSpa = gEnteiPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 0,
        .trainerScale = 370,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_SUICUNE] =
    {
        .categoryName = _("Aurora"),
        .categoryNameSpa = _("Aurora"),
        .height = 20,
        .weight = 1870,
        .description = gSuicunePokedexText,
        .descriptionSpa = gSuicunePokedexTextSpa,
        .pokemonScale = 286,
        .pokemonOffset = 0,
        .trainerScale = 371,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_LARVITAR] =
    {
        .categoryName = _("Rock Skin"),
        .categoryNameSpa = _("Piel Roca"),
        .height = 6,
        .weight = 720,
        .description = gLarvitarPokedexText,
        .descriptionSpa = gLarvitarPokedexTextSpa,
        .pokemonScale = 472,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PUPITAR] =
    {
        .categoryName = _("Hard Shell"),
        .categoryNameSpa = _("Caparazón"),
        .height = 12,
        .weight = 1520,
        .description = gPupitarPokedexText,
        .descriptionSpa = gPupitarPokedexTextSpa,
        .pokemonScale = 292,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TYRANITAR] =
    {
        .categoryName = _("Armor"),
        .categoryNameSpa = _("Coraza"),
        .height = 20,
        .weight = 2020,
        .description = gTyranitarPokedexText,
        .descriptionSpa = gTyranitarPokedexTextSpa,
        .pokemonScale = 285,
        .pokemonOffset = 0,
        .trainerScale = 383,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_LUGIA] =
    {
        .categoryName = _("Diving"),
        .categoryNameSpa = _("Buceo"),
        .height = 52,
        .weight = 2160,
        .description = gLugiaPokedexText,
        .descriptionSpa = gLugiaPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 0,
        .trainerScale = 742,
        .trainerOffset = 18,
    },

    [NATIONAL_DEX_HO_OH] =
    {
        .categoryName = _("Rainbow"),
        .categoryNameSpa = _("Arco Iris"),
        .height = 38,
        .weight = 1990,
        .description = gHoOhPokedexText,
        .descriptionSpa = gHoOhPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 0,
        .trainerScale = 620,
        .trainerOffset = 16,
    },

    [NATIONAL_DEX_CELEBI] =
    {
        .categoryName = _("Time Travel"),
        .categoryNameSpa = _("Viajetiempo"),
        .height = 6,
        .weight = 50,
        .description = gCelebiPokedexText,
        .descriptionSpa = gCelebiPokedexTextSpa,
        .pokemonScale = 393,
        .pokemonOffset = -10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TREECKO] =
    {
        .categoryName = _("Wood Gecko"),
        .categoryNameSpa = _("Geco Bosque"),
        .height = 5,
        .weight = 50,
        .description = gTreeckoPokedexText,
        .descriptionSpa = gTreeckoPokedexTextSpa,
        .pokemonScale = 541,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GROVYLE] =
    {
        .categoryName = _("Wood Gecko"),
        .categoryNameSpa = _("Geco Bosque"),
        .height = 9,
        .weight = 216,
        .description = gGrovylePokedexText,
        .descriptionSpa = gGrovylePokedexTextSpa,
        .pokemonScale = 360,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SCEPTILE] =
    {
        .categoryName = _("Forest"),
        .categoryNameSpa = _("Monte"),
        .height = 17,
        .weight = 522,
        .description = gSceptilePokedexText,
        .descriptionSpa = gSceptilePokedexTextSpa,
        .pokemonScale = 282,
        .pokemonOffset = -1,
        .trainerScale = 313,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_TORCHIC] =
    {
        .categoryName = _("Chick"),
        .categoryNameSpa = _("Polluelo"),
        .height = 4,
        .weight = 25,
        .description = gTorchicPokedexText,
        .descriptionSpa = gTorchicPokedexTextSpa,
        .pokemonScale = 566,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_COMBUSKEN] =
    {
        .categoryName = _("Young Fowl"),
        .categoryNameSpa = _("Ave Corral"),
        .height = 9,
        .weight = 195,
        .description = gCombuskenPokedexText,
        .descriptionSpa = gCombuskenPokedexTextSpa,
        .pokemonScale = 343,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BLAZIKEN] =
    {
        .categoryName = _("Blaze"),
        .categoryNameSpa = _("Llameante"),
        .height = 19,
        .weight = 520,
        .description = gBlazikenPokedexText,
        .descriptionSpa = gBlazikenPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = -1,
        .trainerScale = 314,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_MUDKIP] =
    {
        .categoryName = _("Mud Fish"),
        .categoryNameSpa = _("Pez Lodo"),
        .height = 4,
        .weight = 76,
        .description = gMudkipPokedexText,
        .descriptionSpa = gMudkipPokedexTextSpa,
        .pokemonScale = 535,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MARSHTOMP] =
    {
        .categoryName = _("Mud Fish"),
        .categoryNameSpa = _("Pez Lodo"),
        .height = 7,
        .weight = 280,
        .description = gMarshtompPokedexText,
        .descriptionSpa = gMarshtompPokedexTextSpa,
        .pokemonScale = 340,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SWAMPERT] =
    {
        .categoryName = _("Mud Fish"),
        .categoryNameSpa = _("Pez Lodo"),
        .height = 15,
        .weight = 819,
        .description = gSwampertPokedexText,
        .descriptionSpa = gSwampertPokedexTextSpa,
        .pokemonScale = 276,
        .pokemonOffset = -1,
        .trainerScale = 282,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_POOCHYENA] =
    {
        .categoryName = _("Bite"),
        .categoryNameSpa = _("Mordisco"),
        .height = 5,
        .weight = 136,
        .description = gPoochyenaPokedexText,
        .descriptionSpa = gPoochyenaPokedexTextSpa,
        .pokemonScale = 481,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MIGHTYENA] =
    {
        .categoryName = _("Bite"),
        .categoryNameSpa = _("Mordisco"),
        .height = 10,
        .weight = 370,
        .description = gMightyenaPokedexText,
        .descriptionSpa = gMightyenaPokedexTextSpa,
        .pokemonScale = 359,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ZIGZAGOON] =
    {
        .categoryName = _("Tinyraccoon"),
        .categoryNameSpa = _("Mapachito"),
        .height = 4,
        .weight = 175,
        .description = gZigzagoonPokedexText,
        .descriptionSpa = gZigzagoonPokedexTextSpa,
        .pokemonScale = 560,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LINOONE] =
    {
        .categoryName = _("Rushing"),
        .categoryNameSpa = _("Lanzado"),
        .height = 5,
        .weight = 325,
        .description = gLinoonePokedexText,
        .descriptionSpa = gLinoonePokedexTextSpa,
        .pokemonScale = 321,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WURMPLE] =
    {
        .categoryName = _("Worm"),
        .categoryNameSpa = _("Gusano"),
        .height = 3,
        .weight = 36,
        .description = gWurmplePokedexText,
        .descriptionSpa = gWurmplePokedexTextSpa,
        .pokemonScale = 711,
        .pokemonOffset = 22,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SILCOON] =
    {
        .categoryName = _("Cocoon"),
        .categoryNameSpa = _("Capullo"),
        .height = 6,
        .weight = 100,
        .description = gSilcoonPokedexText,
        .descriptionSpa = gSilcoonPokedexTextSpa,
        .pokemonScale = 431,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BEAUTIFLY] =
    {
        .categoryName = _("Butterfly"),
        .categoryNameSpa = _("Mariposa"),
        .height = 10,
        .weight = 284,
        .description = gBeautiflyPokedexText,
        .descriptionSpa = gBeautiflyPokedexTextSpa,
        .pokemonScale = 298,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CASCOON] =
    {
        .categoryName = _("Cocoon"),
        .categoryNameSpa = _("Capullo"),
        .height = 7,
        .weight = 115,
        .description = gCascoonPokedexText,
        .descriptionSpa = gCascoonPokedexTextSpa,
        .pokemonScale = 391,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DUSTOX] =
    {
        .categoryName = _("Poison Moth"),
        .categoryNameSpa = _("Polilla V."),
        .height = 12,
        .weight = 316,
        .description = gDustoxPokedexText,
        .descriptionSpa = gDustoxPokedexTextSpa,
        .pokemonScale = 269,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LOTAD] =
    {
        .categoryName = _("Water Weed"),
        .categoryNameSpa = _("Alga"),
        .height = 5,
        .weight = 26,
        .description = gLotadPokedexText,
        .descriptionSpa = gLotadPokedexTextSpa,
        .pokemonScale = 406,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LOMBRE] =
    {
        .categoryName = _("Jolly"),
        .categoryNameSpa = _("Alegre"),
        .height = 12,
        .weight = 325,
        .description = gLombrePokedexText,
        .descriptionSpa = gLombrePokedexTextSpa,
        .pokemonScale = 277,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LUDICOLO] =
    {
        .categoryName = _("Carefree"),
        .categoryNameSpa = _("Optimista"),
        .height = 15,
        .weight = 550,
        .description = gLudicoloPokedexText,
        .descriptionSpa = gLudicoloPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = 0,
        .trainerScale = 282,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_SEEDOT] =
    {
        .categoryName = _("Acorn"),
        .categoryNameSpa = _("Bellota"),
        .height = 5,
        .weight = 40,
        .description = gSeedotPokedexText,
        .descriptionSpa = gSeedotPokedexTextSpa,
        .pokemonScale = 472,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NUZLEAF] =
    {
        .categoryName = _("Wily"),
        .categoryNameSpa = _("Astuto"),
        .height = 10,
        .weight = 280,
        .description = gNuzleafPokedexText,
        .descriptionSpa = gNuzleafPokedexTextSpa,
        .pokemonScale = 299,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SHIFTRY] =
    {
        .categoryName = _("Wicked"),
        .categoryNameSpa = _("Malvado"),
        .height = 13,
        .weight = 596,
        .description = gShiftryPokedexText,
        .descriptionSpa = gShiftryPokedexTextSpa,
        .pokemonScale = 290,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TAILLOW] =
    {
        .categoryName = _("Tinyswallow"),
        .categoryNameSpa = _("Pequebuche"),
        .height = 3,
        .weight = 23,
        .description = gTaillowPokedexText,
        .descriptionSpa = gTaillowPokedexTextSpa,
        .pokemonScale = 465,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SWELLOW] =
    {
        .categoryName = _("Swallow"),
        .categoryNameSpa = _("Buche"),
        .height = 7,
        .weight = 198,
        .description = gSwellowPokedexText,
        .descriptionSpa = gSwellowPokedexTextSpa,
        .pokemonScale = 428,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WINGULL] =
    {
        .categoryName = _("Seagull"),
        .categoryNameSpa = _("Gaviota"),
        .height = 6,
        .weight = 95,
        .description = gWingullPokedexText,
        .descriptionSpa = gWingullPokedexTextSpa,
        .pokemonScale = 295,
        .pokemonOffset = -2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PELIPPER] =
    {
        .categoryName = _("Water Bird"),
        .categoryNameSpa = _("Ave Agua"),
        .height = 12,
        .weight = 280,
        .description = gPelipperPokedexText,
        .descriptionSpa = gPelipperPokedexTextSpa,
        .pokemonScale = 288,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RALTS] =
    {
        .categoryName = _("Feeling"),
        .categoryNameSpa = _("Sensible"),
        .height = 4,
        .weight = 66,
        .description = gRaltsPokedexText,
        .descriptionSpa = gRaltsPokedexTextSpa,
        .pokemonScale = 457,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KIRLIA] =
    {
        .categoryName = _("Emotion"),
        .categoryNameSpa = _("Sensorio"),
        .height = 8,
        .weight = 202,
        .description = gKirliaPokedexText,
        .descriptionSpa = gKirliaPokedexTextSpa,
        .pokemonScale = 354,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GARDEVOIR] =
    {
        .categoryName = _("Embrace"),
        .categoryNameSpa = _("Envolvente"),
        .height = 16,
        .weight = 484,
        .description = gGardevoirPokedexText,
        .descriptionSpa = gGardevoirPokedexTextSpa,
        .pokemonScale = 277,
        .pokemonOffset = 0,
        .trainerScale = 276,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_SURSKIT] =
    {
        .categoryName = _("Pond Skater"),
        .categoryNameSpa = _("Zapatero"),
        .height = 5,
        .weight = 17,
        .description = gSurskitPokedexText,
        .descriptionSpa = gSurskitPokedexTextSpa,
        .pokemonScale = 375,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MASQUERAIN] =
    {
        .categoryName = _("Eyeball"),
        .categoryNameSpa = _("Globocular"),
        .height = 8,
        .weight = 36,
        .description = gMasquerainPokedexText,
        .descriptionSpa = gMasquerainPokedexTextSpa,
        .pokemonScale = 378,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SHROOMISH] =
    {
        .categoryName = _("Mushroom"),
        .categoryNameSpa = _("Hongo"),
        .height = 4,
        .weight = 45,
        .description = gShroomishPokedexText,
        .descriptionSpa = gShroomishPokedexTextSpa,
        .pokemonScale = 513,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BRELOOM] =
    {
        .categoryName = _("Mushroom"),
        .categoryNameSpa = _("Hongo"),
        .height = 12,
        .weight = 392,
        .description = gBreloomPokedexText,
        .descriptionSpa = gBreloomPokedexTextSpa,
        .pokemonScale = 324,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SLAKOTH] =
    {
        .categoryName = _("Slacker"),
        .categoryNameSpa = _("Gandul"),
        .height = 8,
        .weight = 240,
        .description = gSlakothPokedexText,
        .descriptionSpa = gSlakothPokedexTextSpa,
        .pokemonScale = 291,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VIGOROTH] =
    {
        .categoryName = _("Wild Monkey"),
        .categoryNameSpa = _("Mono Feroz"),
        .height = 14,
        .weight = 465,
        .description = gVigorothPokedexText,
        .descriptionSpa = gVigorothPokedexTextSpa,
        .pokemonScale = 301,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SLAKING] =
    {
        .categoryName = _("Lazy"),
        .categoryNameSpa = _("Holgazán"),
        .height = 20,
        .weight = 1305,
        .description = gSlakingPokedexText,
        .descriptionSpa = gSlakingPokedexTextSpa,
        .pokemonScale = 277,
        .pokemonOffset = 5,
        .trainerScale = 326,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_NINCADA] =
    {
        .categoryName = _("Trainee"),
        .categoryNameSpa = _("Aprendiz"),
        .height = 5,
        .weight = 55,
        .description = gNincadaPokedexText,
        .descriptionSpa = gNincadaPokedexTextSpa,
        .pokemonScale = 405,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NINJASK] =
    {
        .categoryName = _("Ninja"),
        .categoryNameSpa = _("Ninja"),
        .height = 8,
        .weight = 120,
        .description = gNinjaskPokedexText,
        .descriptionSpa = gNinjaskPokedexTextSpa,
        .pokemonScale = 383,
        .pokemonOffset = -9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SHEDINJA] =
    {
        .categoryName = _("Shed"),
        .categoryNameSpa = _("Muda"),
        .height = 8,
        .weight = 12,
        .description = gShedinjaPokedexText,
        .descriptionSpa = gShedinjaPokedexTextSpa,
        .pokemonScale = 372,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WHISMUR] =
    {
        .categoryName = _("Whisper"),
        .categoryNameSpa = _("Susurro"),
        .height = 6,
        .weight = 163,
        .description = gWhismurPokedexText,
        .descriptionSpa = gWhismurPokedexTextSpa,
        .pokemonScale = 373,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LOUDRED] =
    {
        .categoryName = _("Big Voice"),
        .categoryNameSpa = _("Chillón"),
        .height = 10,
        .weight = 405,
        .description = gLoudredPokedexText,
        .descriptionSpa = gLoudredPokedexTextSpa,
        .pokemonScale = 356,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_EXPLOUD] =
    {
        .categoryName = _("Loud Noise"),
        .categoryNameSpa = _("Escandaloso"),
        .height = 15,
        .weight = 840,
        .description = gExploudPokedexText,
        .descriptionSpa = gExploudPokedexTextSpa,
        .pokemonScale = 284,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAKUHITA] =
    {
        .categoryName = _("Guts"),
        .categoryNameSpa = _("Valiente"),
        .height = 10,
        .weight = 864,
        .description = gMakuhitaPokedexText,
        .descriptionSpa = gMakuhitaPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HARIYAMA] =
    {
        .categoryName = _("Arm Thrust"),
        .categoryNameSpa = _("Empuje"),
        .height = 23,
        .weight = 2538,
        .description = gHariyamaPokedexText,
        .descriptionSpa = gHariyamaPokedexTextSpa,
        .pokemonScale = 268,
        .pokemonOffset = -1,
        .trainerScale = 375,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_AZURILL] =
    {
        .categoryName = _("Polka Dot"),
        .categoryNameSpa = _("A Topos"),
        .height = 2,
        .weight = 20,
        .description = gAzurillPokedexText,
        .descriptionSpa = gAzurillPokedexTextSpa,
        .pokemonScale = 603,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_NOSEPASS] =
    {
        .categoryName = _("Compass"),
        .categoryNameSpa = _("Brújula"),
        .height = 10,
        .weight = 970,
        .description = gNosepassPokedexText,
        .descriptionSpa = gNosepassPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 9,
        .trainerScale = 289,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_SKITTY] =
    {
        .categoryName = _("Kitten"),
        .categoryNameSpa = _("Gatito"),
        .height = 6,
        .weight = 110,
        .description = gSkittyPokedexText,
        .descriptionSpa = gSkittyPokedexTextSpa,
        .pokemonScale = 492,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DELCATTY] =
    {
        .categoryName = _("Prim"),
        .categoryNameSpa = _("Cursi"),
        .height = 11,
        .weight = 326,
        .description = gDelcattyPokedexText,
        .descriptionSpa = gDelcattyPokedexTextSpa,
        .pokemonScale = 322,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SABLEYE] =
    {
        .categoryName = _("Darkness"),
        .categoryNameSpa = _("Oscuridad"),
        .height = 5,
        .weight = 110,
        .description = gSableyePokedexText,
        .descriptionSpa = gSableyePokedexTextSpa,
        .pokemonScale = 451,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MAWILE] =
    {
        .categoryName = _("Deceiver"),
        .categoryNameSpa = _("Tramposo"),
        .height = 6,
        .weight = 115,
        .description = gMawilePokedexText,
        .descriptionSpa = gMawilePokedexTextSpa,
        .pokemonScale = 466,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -3,
    },

    [NATIONAL_DEX_ARON] =
    {
        .categoryName = _("Iron Armor"),
        .categoryNameSpa = _("Cor. Férrea"),
        .height = 4,
        .weight = 600,
        .description = gAronPokedexText,
        .descriptionSpa = gAronPokedexTextSpa,
        .pokemonScale = 419,
        .pokemonOffset = 21,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LAIRON] =
    {
        .categoryName = _("Iron Armor"),
        .categoryNameSpa = _("Cor. Férrea"),
        .height = 9,
        .weight = 1200,
        .description = gLaironPokedexText,
        .descriptionSpa = gLaironPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_AGGRON] =
    {
        .categoryName = _("Iron Armor"),
        .categoryNameSpa = _("Cor. Férrea"),
        .height = 21,
        .weight = 3600,
        .description = gAggronPokedexText,
        .descriptionSpa = gAggronPokedexTextSpa,
        .pokemonScale = 274,
        .pokemonOffset = -1,
        .trainerScale = 374,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_MEDITITE] =
    {
        .categoryName = _("Meditate"),
        .categoryNameSpa = _("Meditador"),
        .height = 6,
        .weight = 112,
        .description = gMedititePokedexText,
        .descriptionSpa = gMedititePokedexTextSpa,
        .pokemonScale = 465,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MEDICHAM] =
    {
        .categoryName = _("Meditate"),
        .categoryNameSpa = _("Meditador"),
        .height = 13,
        .weight = 315,
        .description = gMedichamPokedexText,
        .descriptionSpa = gMedichamPokedexTextSpa,
        .pokemonScale = 298,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ELECTRIKE] =
    {
        .categoryName = _("Lightning"),
        .categoryNameSpa = _("Relámpago"),
        .height = 6,
        .weight = 152,
        .description = gElectrikePokedexText,
        .descriptionSpa = gElectrikePokedexTextSpa,
        .pokemonScale = 290,
        .pokemonOffset = 16,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MANECTRIC] =
    {
        .categoryName = _("Discharge"),
        .categoryNameSpa = _("Descarga"),
        .height = 15,
        .weight = 402,
        .description = gManectricPokedexText,
        .descriptionSpa = gManectricPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 257,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_PLUSLE] =
    {
        .categoryName = _("Cheering"),
        .categoryNameSpa = _("Ánimo"),
        .height = 4,
        .weight = 42,
        .description = gPluslePokedexText,
        .descriptionSpa = gPluslePokedexTextSpa,
        .pokemonScale = 515,
        .pokemonOffset = -9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MINUN] =
    {
        .categoryName = _("Cheering"),
        .categoryNameSpa = _("Ánimo"),
        .height = 4,
        .weight = 42,
        .description = gMinunPokedexText,
        .descriptionSpa = gMinunPokedexTextSpa,
        .pokemonScale = 512,
        .pokemonOffset = -7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VOLBEAT] =
    {
        .categoryName = _("Firefly"),
        .categoryNameSpa = _("Luciérnaga"),
        .height = 7,
        .weight = 177,
        .description = gVolbeatPokedexText,
        .descriptionSpa = gVolbeatPokedexTextSpa,
        .pokemonScale = 442,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ILLUMISE] =
    {
        .categoryName = _("Firefly"),
        .categoryNameSpa = _("Luciérnaga"),
        .height = 6,
        .weight = 177,
        .description = gIllumisePokedexText,
        .descriptionSpa = gIllumisePokedexTextSpa,
        .pokemonScale = 572,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ROSELIA] =
    {
        .categoryName = _("Thorn"),
        .categoryNameSpa = _("Espina"),
        .height = 3,
        .weight = 20,
        .description = gRoseliaPokedexText,
        .descriptionSpa = gRoseliaPokedexTextSpa,
        .pokemonScale = 677,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GULPIN] =
    {
        .categoryName = _("Stomach"),
        .categoryNameSpa = _("Estómago"),
        .height = 4,
        .weight = 103,
        .description = gGulpinPokedexText,
        .descriptionSpa = gGulpinPokedexTextSpa,
        .pokemonScale = 593,
        .pokemonOffset = 22,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SWALOT] =
    {
        .categoryName = _("Poison Bag"),
        .categoryNameSpa = _("Bolsa Ven."),
        .height = 17,
        .weight = 800,
        .description = gSwalotPokedexText,
        .descriptionSpa = gSwalotPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 5,
        .trainerScale = 345,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_CARVANHA] =
    {
        .categoryName = _("Savage"),
        .categoryNameSpa = _("Feroz"),
        .height = 8,
        .weight = 208,
        .description = gCarvanhaPokedexText,
        .descriptionSpa = gCarvanhaPokedexTextSpa,
        .pokemonScale = 362,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SHARPEDO] =
    {
        .categoryName = _("Brutal"),
        .categoryNameSpa = _("Voraz"),
        .height = 18,
        .weight = 888,
        .description = gSharpedoPokedexText,
        .descriptionSpa = gSharpedoPokedexTextSpa,
        .pokemonScale = 265,
        .pokemonOffset = 0,
        .trainerScale = 342,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_WAILMER] =
    {
        .categoryName = _("Ball Whale"),
        .categoryNameSpa = _("Ballenabola"),
        .height = 20,
        .weight = 1300,
        .description = gWailmerPokedexText,
        .descriptionSpa = gWailmerPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 10,
        .trainerScale = 493,
        .trainerOffset = 10,
    },

    [NATIONAL_DEX_WAILORD] =
    {
        .categoryName = _("Float Whale"),
        .categoryNameSpa = _("Ballenaflot"),
        .height = 145,
        .weight = 3980,
        .description = gWailordPokedexText,
        .descriptionSpa = gWailordPokedexTextSpa,
        .pokemonScale = 276,
        .pokemonOffset = -1,
        .trainerScale = 1428,
        .trainerOffset = 20,
    },

    [NATIONAL_DEX_NUMEL] =
    {
        .categoryName = _("Numb"),
        .categoryNameSpa = _("Torpe"),
        .height = 7,
        .weight = 240,
        .description = gNumelPokedexText,
        .descriptionSpa = gNumelPokedexTextSpa,
        .pokemonScale = 310,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CAMERUPT] =
    {
        .categoryName = _("Eruption"),
        .categoryNameSpa = _("Erupción"),
        .height = 19,
        .weight = 2200,
        .description = gCameruptPokedexText,
        .descriptionSpa = gCameruptPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 6,
        .trainerScale = 345,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_TORKOAL] =
    {
        .categoryName = _("Coal"),
        .categoryNameSpa = _("Carbón"),
        .height = 5,
        .weight = 804,
        .description = gTorkoalPokedexText,
        .descriptionSpa = gTorkoalPokedexTextSpa,
        .pokemonScale = 392,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SPOINK] =
    {
        .categoryName = _("Bounce"),
        .categoryNameSpa = _("Saltarín"),
        .height = 7,
        .weight = 306,
        .description = gSpoinkPokedexText,
        .descriptionSpa = gSpoinkPokedexTextSpa,
        .pokemonScale = 423,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GRUMPIG] =
    {
        .categoryName = _("Manipulate"),
        .categoryNameSpa = _("Manipulador"),
        .height = 9,
        .weight = 715,
        .description = gGrumpigPokedexText,
        .descriptionSpa = gGrumpigPokedexTextSpa,
        .pokemonScale = 358,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SPINDA] =
    {
        .categoryName = _("Spot Panda"),
        .categoryNameSpa = _("Panda Topos"),
        .height = 11,
        .weight = 50,
        .description = gSpindaPokedexText,
        .descriptionSpa = gSpindaPokedexTextSpa,
        .pokemonScale = 321,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_TRAPINCH] =
    {
        .categoryName = _("Ant Pit"),
        .categoryNameSpa = _("Hormigaleón"),
        .height = 7,
        .weight = 150,
        .description = gTrapinchPokedexText,
        .descriptionSpa = gTrapinchPokedexTextSpa,
        .pokemonScale = 298,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_VIBRAVA] =
    {
        .categoryName = _("Vibration"),
        .categoryNameSpa = _("Vibrante"),
        .height = 11,
        .weight = 153,
        .description = gVibravaPokedexText,
        .descriptionSpa = gVibravaPokedexTextSpa,
        .pokemonScale = 370,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_FLYGON] =
    {
        .categoryName = _("Mystic"),
        .categoryNameSpa = _("Místico"),
        .height = 20,
        .weight = 820,
        .description = gFlygonPokedexText,
        .descriptionSpa = gFlygonPokedexTextSpa,
        .pokemonScale = 280,
        .pokemonOffset = 0,
        .trainerScale = 299,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_CACNEA] =
    {
        .categoryName = _("Cactus"),
        .categoryNameSpa = _("Cactus"),
        .height = 4,
        .weight = 513,
        .description = gCacneaPokedexText,
        .descriptionSpa = gCacneaPokedexTextSpa,
        .pokemonScale = 455,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CACTURNE] =
    {
        .categoryName = _("Scarecrow"),
        .categoryNameSpa = _("Espantajo"),
        .height = 13,
        .weight = 774,
        .description = gCacturnePokedexText,
        .descriptionSpa = gCacturnePokedexTextSpa,
        .pokemonScale = 327,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SWABLU] =
    {
        .categoryName = _("Cotton Bird"),
        .categoryNameSpa = _("Ave Algodón"),
        .height = 4,
        .weight = 12,
        .description = gSwabluPokedexText,
        .descriptionSpa = gSwabluPokedexTextSpa,
        .pokemonScale = 422,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ALTARIA] =
    {
        .categoryNameSpa = _("Cantor"),
        .categoryName = _("Humming"),
        .height = 11,
        .weight = 206,
        .description = gAltariaPokedexText,
        .descriptionSpa = gAltariaPokedexTextSpa,
        .pokemonScale = 327,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ZANGOOSE] =
    {
        .categoryName = _("Cat Ferret"),
        .categoryNameSpa = _("Gato Hurón"),
        .height = 13,
        .weight = 403,
        .description = gZangoosePokedexText,
        .descriptionSpa = gZangoosePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SEVIPER] =
    {
        .categoryName = _("Fang Snake"),
        .categoryNameSpa = _("Colmillos"),
        .height = 27,
        .weight = 525,
        .description = gSeviperPokedexText,
        .descriptionSpa = gSeviperPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LUNATONE] =
    {
        .categoryName = _("Meteorite"),
        .categoryNameSpa = _("Meteorito"),
        .height = 10,
        .weight = 1680,
        .description = gLunatonePokedexText,
        .descriptionSpa = gLunatonePokedexTextSpa,
        .pokemonScale = 300,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SOLROCK] =
    {
        .categoryName = _("Meteorite"),
        .categoryNameSpa = _("Meteorito"),
        .height = 12,
        .weight = 1540,
        .description = gSolrockPokedexText,
        .descriptionSpa = gSolrockPokedexTextSpa,
        .pokemonScale = 328,
        .pokemonOffset = -3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BARBOACH] =
    {
        .categoryName = _("Whiskers"),
        .categoryNameSpa = _("Bigotudo"),
        .height = 4,
        .weight = 19,
        .description = gBarboachPokedexText,
        .descriptionSpa = gBarboachPokedexTextSpa,
        .pokemonScale = 581,
        .pokemonOffset = -2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WHISCASH] =
    {
        .categoryName = _("Whiskers"),
        .categoryNameSpa = _("Bigotudo"),
        .height = 9,
        .weight = 236,
        .description = gWhiscashPokedexText,
        .descriptionSpa = gWhiscashPokedexTextSpa,
        .pokemonScale = 317,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CORPHISH] =
    {
        .categoryName = _("Ruffian"),
        .categoryNameSpa = _("Rifián"),
        .height = 6,
        .weight = 115,
        .description = gCorphishPokedexText,
        .descriptionSpa = gCorphishPokedexTextSpa,
        .pokemonScale = 484,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CRAWDAUNT] =
    {
        .categoryName = _("Rogue"),
        .categoryNameSpa = _("Granuja"),
        .height = 11,
        .weight = 328,
        .description = gCrawdauntPokedexText,
        .descriptionSpa = gCrawdauntPokedexTextSpa,
        .pokemonScale = 365,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BALTOY] =
    {
        .categoryName = _("Clay Doll"),
        .categoryNameSpa = _("Muñ. Barro"),
        .height = 5,
        .weight = 215,
        .description = gBaltoyPokedexText,
        .descriptionSpa = gBaltoyPokedexTextSpa,
        .pokemonScale = 384,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = -3,
    },

    [NATIONAL_DEX_CLAYDOL] =
    {
        .categoryName = _("Clay Doll"),
        .categoryNameSpa = _("Muñ. Barro"),
        .height = 15,
        .weight = 1080,
        .description = gClaydolPokedexText,
        .descriptionSpa = gClaydolPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 280,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_LILEEP] =
    {
        .categoryName = _("Sea Lily"),
        .categoryNameSpa = _("Lila Mar"),
        .height = 10,
        .weight = 238,
        .description = gLileepPokedexText,
        .descriptionSpa = gLileepPokedexTextSpa,
        .pokemonScale = 305,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_CRADILY] =
    {
        .categoryName = _("Barnacle"),
        .categoryNameSpa = _("Percebe"),
        .height = 15,
        .weight = 604,
        .description = gCradilyPokedexText,
        .descriptionSpa = gCradilyPokedexTextSpa,
        .pokemonScale = 275,
        .pokemonOffset = -1,
        .trainerScale = 269,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_ANORITH] =
    {
        .categoryName = _("Old Shrimp"),
        .categoryNameSpa = _("Camarón"),
        .height = 7,
        .weight = 125,
        .description = gAnorithPokedexText,
        .descriptionSpa = gAnorithPokedexTextSpa,
        .pokemonScale = 296,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ARMALDO] =
    {
        .categoryName = _("Plate"),
        .categoryNameSpa = _("Armadura"),
        .height = 15,
        .weight = 682,
        .description = gArmaldoPokedexText,
        .descriptionSpa = gArmaldoPokedexTextSpa,
        .pokemonScale = 312,
        .pokemonOffset = 2,
        .trainerScale = 271,
        .trainerOffset = -1,
    },

    [NATIONAL_DEX_FEEBAS] =
    {
        .categoryName = _("Fish"),
        .categoryNameSpa = _("Pez"),
        .height = 6,
        .weight = 74,
        .description = gFeebasPokedexText,
        .descriptionSpa = gFeebasPokedexTextSpa,
        .pokemonScale = 423,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_MILOTIC] =
    {
        .categoryName = _("Tender"),
        .categoryNameSpa = _("Tierno"),
        .height = 62,
        .weight = 1620,
        .description = gMiloticPokedexText,
        .descriptionSpa = gMiloticPokedexTextSpa,
        .pokemonScale = 282,
        .pokemonOffset = -1,
        .trainerScale = 382,
        .trainerOffset = 7,
    },

    [NATIONAL_DEX_CASTFORM] =
    {
        .categoryName = _("Weather"),
        .categoryNameSpa = _("Clima"),
        .height = 3,
        .weight = 8,
        .description = gCastformPokedexText,
        .descriptionSpa = gCastformPokedexTextSpa,
        .pokemonScale = 435,
        .pokemonOffset = -5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_KECLEON] =
    {
        .categoryName = _("Color Swap"),
        .categoryNameSpa = _("Camaleónico"),
        .height = 10,
        .weight = 220,
        .description = gKecleonPokedexText,
        .descriptionSpa = gKecleonPokedexTextSpa,
        .pokemonScale = 316,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SHUPPET] =
    {
        .categoryName = _("Puppet"),
        .categoryNameSpa = _("Títere"),
        .height = 6,
        .weight = 23,
        .description = gShuppetPokedexText,
        .descriptionSpa = gShuppetPokedexTextSpa,
        .pokemonScale = 440,
        .pokemonOffset = -3,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BANETTE] =
    {
        .categoryName = _("Marionette"),
        .categoryNameSpa = _("Marioneta"),
        .height = 11,
        .weight = 125,
        .description = gBanettePokedexText,
        .descriptionSpa = gBanettePokedexTextSpa,
        .pokemonScale = 262,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DUSKULL] =
    {
        .categoryName = _("Requiem"),
        .categoryNameSpa = _("Réquiem"),
        .height = 8,
        .weight = 150,
        .description = gDuskullPokedexText,
        .descriptionSpa = gDuskullPokedexTextSpa,
        .pokemonScale = 376,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DUSCLOPS] =
    {
        .categoryName = _("Beckon"),
        .categoryNameSpa = _("Atrayente"),
        .height = 16,
        .weight = 306,
        .description = gDusclopsPokedexText,
        .descriptionSpa = gDusclopsPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 299,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_TROPIUS] =
    {
        .categoryName = _("Fruit"),
        .categoryNameSpa = _("Fruto"),
        .height = 20,
        .weight = 1000,
        .description = gTropiusPokedexText,
        .descriptionSpa = gTropiusPokedexTextSpa,
        .pokemonScale = 283,
        .pokemonOffset = -1,
        .trainerScale = 371,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_CHIMECHO] =
    {
        .categoryName = _("Wind Chime"),
        .categoryNameSpa = _("Sonido Aire"),
        .height = 6,
        .weight = 10,
        .description = gChimechoPokedexText,
        .descriptionSpa = gChimechoPokedexTextSpa,
        .pokemonScale = 505,
        .pokemonOffset = -4,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_ABSOL] =
    {
        .categoryName = _("Disaster"),
        .categoryNameSpa = _("Catástrofe"),
        .height = 12,
        .weight = 470,
        .description = gAbsolPokedexText,
        .descriptionSpa = gAbsolPokedexTextSpa,
        .pokemonScale = 301,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WYNAUT] =
    {
        .categoryName = _("Bright"),
        .categoryNameSpa = _("Radiante"),
        .height = 6,
        .weight = 140,
        .description = gWynautPokedexText,
        .descriptionSpa = gWynautPokedexTextSpa,
        .pokemonScale = 453,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SNORUNT] =
    {
        .categoryName = _("Snow Hat"),
        .categoryNameSpa = _("Gorro Nieve"),
        .height = 7,
        .weight = 168,
        .description = gSnoruntPokedexText,
        .descriptionSpa = gSnoruntPokedexTextSpa,
        .pokemonScale = 380,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GLALIE] =
    {
        .categoryName = _("Face"),
        .categoryNameSpa = _("Cara"),
        .height = 15,
        .weight = 2565,
        .description = gGlaliePokedexText,
        .descriptionSpa = gGlaliePokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 344,
        .trainerOffset = 2,
    },

    [NATIONAL_DEX_SPHEAL] =
    {
        .categoryName = _("Clap"),
        .categoryNameSpa = _("Aplauso"),
        .height = 8,
        .weight = 395,
        .description = gSphealPokedexText,
        .descriptionSpa = gSphealPokedexTextSpa,
        .pokemonScale = 315,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SEALEO] =
    {
        .categoryName = _("Ball Roll"),
        .categoryNameSpa = _("Rodabola"),
        .height = 11,
        .weight = 876,
        .description = gSealeoPokedexText,
        .descriptionSpa = gSealeoPokedexTextSpa,
        .pokemonScale = 338,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_WALREIN] =
    {
        .categoryName = _("Ice Break"),
        .categoryNameSpa = _("Rompehielo"),
        .height = 14,
        .weight = 1506,
        .description = gWalreinPokedexText,
        .descriptionSpa = gWalreinPokedexTextSpa,
        .pokemonScale = 305,
        .pokemonOffset = 2,
        .trainerScale = 277,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_CLAMPERL] =
    {
        .categoryName = _("Bivalve"),
        .categoryNameSpa = _("Bivalvo"),
        .height = 4,
        .weight = 525,
        .description = gClamperlPokedexText,
        .descriptionSpa = gClamperlPokedexTextSpa,
        .pokemonScale = 691,
        .pokemonOffset = 22,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_HUNTAIL] =
    {
        .categoryName = _("Deep Sea"),
        .categoryNameSpa = _("Abisal"),
        .height = 17,
        .weight = 270,
        .description = gHuntailPokedexText,
        .descriptionSpa = gHuntailPokedexTextSpa,
        .pokemonScale = 307,
        .pokemonOffset = 1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_GOREBYSS] =
    {
        .categoryName = _("South Sea"),
        .categoryNameSpa = _("Mar del Sur"),
        .height = 18,
        .weight = 226,
        .description = gGorebyssPokedexText,
        .descriptionSpa = gGorebyssPokedexTextSpa,
        .pokemonScale = 278,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_RELICANTH] =
    {
        .categoryName = _("Longevity"),
        .categoryNameSpa = _("Longevo"),
        .height = 10,
        .weight = 234,
        .description = gRelicanthPokedexText,
        .descriptionSpa = gRelicanthPokedexTextSpa,
        .pokemonScale = 316,
        .pokemonOffset = 5,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LUVDISC] =
    {
        .categoryName = _("Rendezvous"),
        .categoryNameSpa = _("Cita"),
        .height = 6,
        .weight = 87,
        .description = gLuvdiscPokedexText,
        .descriptionSpa = gLuvdiscPokedexTextSpa,
        .pokemonScale = 371,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_BAGON] =
    {
        .categoryName = _("Rock Head"),
        .categoryNameSpa = _("Cabeza Roca"),
        .height = 6,
        .weight = 421,
        .description = gBagonPokedexText,
        .descriptionSpa = gBagonPokedexTextSpa,
        .pokemonScale = 448,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SHELGON] =
    {
        .categoryName = _("Endurance"),
        .categoryNameSpa = _("Resistencia"),
        .height = 11,
        .weight = 1105,
        .description = gShelgonPokedexText,
        .descriptionSpa = gShelgonPokedexTextSpa,
        .pokemonScale = 311,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_SALAMENCE] =
    {
        .categoryName = _("Dragon"),
        .categoryNameSpa = _("Dragón"),
        .height = 15,
        .weight = 1026,
        .description = gSalamencePokedexText,
        .descriptionSpa = gSalamencePokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 2,
        .trainerScale = 307,
        .trainerOffset = 0,
    },

    [NATIONAL_DEX_BELDUM] =
    {
        .categoryName = _("Iron Ball"),
        .categoryNameSpa = _("Bola Hierro"),
        .height = 6,
        .weight = 952,
        .description = gBeldumPokedexText,
        .descriptionSpa = gBeldumPokedexTextSpa,
        .pokemonScale = 414,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_METANG] =
    {
        .categoryName = _("Iron Claw"),
        .categoryNameSpa = _("Garrahierro"),
        .height = 12,
        .weight = 2025,
        .description = gMetangPokedexText,
        .descriptionSpa = gMetangPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 272,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_METAGROSS] =
    {
        .categoryName = _("Iron Leg"),
        .categoryNameSpa = _("Pata Hierro"),
        .height = 16,
        .weight = 5500,
        .description = gMetagrossPokedexText,
        .descriptionSpa = gMetagrossPokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 3,
        .trainerScale = 461,
        .trainerOffset = 4,
    },

    [NATIONAL_DEX_REGIROCK] =
    {
        .categoryName = _("Rock Peak"),
        .categoryNameSpa = _("Pico Roca"),
        .height = 17,
        .weight = 2300,
        .description = gRegirockPokedexText,
        .descriptionSpa = gRegirockPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 309,
        .trainerOffset = 1,
    },

    [NATIONAL_DEX_REGICE] =
    {
        .categoryName = _("Iceberg"),
        .categoryNameSpa = _("Iceberg"),
        .height = 18,
        .weight = 1750,
        .description = gRegicePokedexText,
        .descriptionSpa = gRegicePokedexTextSpa,
        .pokemonScale = 265,
        .pokemonOffset = 0,
        .trainerScale = 317,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_REGISTEEL] =
    {
        .categoryName = _("Iron"),
        .categoryNameSpa = _("Hierro"),
        .height = 19,
        .weight = 2050,
        .description = gRegisteelPokedexText,
        .descriptionSpa = gRegisteelPokedexTextSpa,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 359,
        .trainerOffset = 6,
    },

    [NATIONAL_DEX_LATIAS] =
    {
        .categoryName = _("Eon"),
        .categoryNameSpa = _("Eón"),
        .height = 14,
        .weight = 400,
        .description = gLatiasPokedexText,
        .descriptionSpa = gLatiasPokedexTextSpa,
        .pokemonScale = 291,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_LATIOS] =
    {
        .categoryName = _("Eon"),
        .categoryNameSpa = _("Eón"),
        .height = 20,
        .weight = 600,
        .description = gLatiosPokedexText,
        .descriptionSpa = gLatiosPokedexTextSpa,
        .pokemonScale = 273,
        .pokemonOffset = 0,
        .trainerScale = 313,
        .trainerOffset = 3,
    },

    [NATIONAL_DEX_KYOGRE] =
    {
        .categoryName = _("Sea Basin"),
        .categoryNameSpa = _("Cuenca Mar"),
        .height = 45,
        .weight = 3520,
        .description = gKyogrePokedexText,
        .descriptionSpa = gKyogrePokedexTextSpa,
        .pokemonScale = 272,
        .pokemonOffset = 1,
        .trainerScale = 639,
        .trainerOffset = 13,
    },

    [NATIONAL_DEX_GROUDON] =
    {
        .categoryName = _("Continent"),
        .categoryNameSpa = _("Continente"),
        .height = 35,
        .weight = 9500,
        .description = gGroudonPokedexText,
        .descriptionSpa = gGroudonPokedexTextSpa,
        .pokemonScale = 276,
        .pokemonOffset = 0,
        .trainerScale = 530,
        .trainerOffset = 12,
    },

    [NATIONAL_DEX_RAYQUAZA] =
    {
        .categoryName = _("Sky High"),
        .categoryNameSpa = _("Cielo"),
        .height = 70,
        .weight = 2065,
        .description = gRayquazaPokedexText,
        .descriptionSpa = gRayquazaPokedexTextSpa,
        .pokemonScale = 286,
        .pokemonOffset = -1,
        .trainerScale = 483,
        .trainerOffset = 9,
    },

    [NATIONAL_DEX_JIRACHI] =
    {
        .categoryName = _("Wish"),
        .categoryNameSpa = _("Deseo"),
        .height = 3,
        .weight = 11,
        .description = gJirachiPokedexText,
        .descriptionSpa = gJirachiPokedexTextSpa,
        .pokemonScale = 608,
        .pokemonOffset = -8,
        .trainerScale = 256,
        .trainerOffset = -2,
    },

    [NATIONAL_DEX_DEOXYS] =
    {
        .categoryName = _("DNA"),
        .categoryNameSpa = _("ADN"),
        .height = 17,
        .weight = 608,
        .description = gDeoxysPokedexText,
        .descriptionSpa = gDeoxysPokedexTextSpa,
        .pokemonScale = 293,
        .pokemonOffset = 0,
        .trainerScale = 337,
        .trainerOffset = 2,
    },
};
