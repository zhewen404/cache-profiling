import plotly.express as px
scheme_to_color = {
    "XORCache+BDI randSet": px.colors.qualitative.Vivid[4],
    "XOR randBank+BDI": px.colors.sequential.Teal[1],

    "XOR randBank+BPC": px.colors.sequential.Emrld[1],

    "XOR idealSet+BDI": px.colors.sequential.Teal[2],
    "XOR idealSet-0+BDI": px.colors.sequential.Teal[2],
    "XOR idealSet-1+BDI": px.colors.sequential.Teal[3],
    "XOR idealSet-2+BDI": "#5C9EAF",
    "XOR idealSet-3+BDI": px.colors.sequential.Teal[4],
    "XOR idealSet-4+BDI": px.colors.sequential.Teal[5],

    "XOR idealSet+BPC": px.colors.sequential.Emrld[2],
    "XOR idealSet-0+BPC": px.colors.sequential.Emrld[2],
    "XOR idealSet-1+BPC": px.colors.sequential.Emrld[3],
    "XOR idealSet-2+BPC": "#368A7E",
    "XOR idealSet-3+BPC": px.colors.sequential.Emrld[4],
    "XOR idealSet-4+BPC": px.colors.sequential.Emrld[5],

    "XOR idealBank+BDI": px.colors.sequential.Teal[6],
    "XOR idealBank+BPC": px.colors.sequential.Emrld[6],
    "oracle": px.colors.qualitative.Vivid[4],
    "idealBank": px.colors.qualitative.Vivid[4],

    "bpc": px.colors.qualitative.Vivid[9],
    "Bitplane (no XOR)": px.colors.qualitative.Vivid[9],
    "BPC (no XOR)": px.colors.sequential.Emrld[0],

    "BDI (no XOR)": px.colors.sequential.Teal[0],
    "BDI (no XOR) ": px.colors.qualitative.Vivid[9],
    # "bdi": px.colors.qualitative.Pastel[4],
    "bdi": px.colors.qualitative.Vivid[9],
    "BDI": px.colors.qualitative.Vivid[9],
    "bdi-immo": px.colors.qualitative.Vivid[9],

    "thesaurus-flat": px.colors.qualitative.Vivid[2],
    "XORCache(LSH-RP)": px.colors.qualitative.Vivid[2],
    "XOR(LSH-RP)+BDI": px.colors.qualitative.Vivid[2],
    "XORCache+BPC(LSH-RP)": px.colors.qualitative.Vivid[2],
    "thesaurus-immo": px.colors.qualitative.Vivid[2],

    "shuffle-xorfold": px.colors.qualitative.Vivid[8],

    "bit-sampling-flat": px.colors.qualitative.Vivid[3],
    "XORCache(LSH-BS)": px.colors.qualitative.Vivid[3],
    "XOR(LSH-BS)+BDI": px.colors.qualitative.Vivid[3],
    "XORCache+BPC(LSH-BS)": px.colors.qualitative.Vivid[3],
    "masked-bit-sampling_8_32": px.colors.qualitative.Vivid[3],
    "masked-bit-sampling_4_16": px.colors.qualitative.Vivid[3],
    "masked-bit-sampling_8_16": px.colors.qualitative.Vivid[3],
    "masked-bit-sampling_8_16 (BPC)": px.colors.qualitative.Vivid[3],
    "masked-bit-sampling_4_8": px.colors.qualitative.Vivid[3],

    "byte labeling-flat": px.colors.qualitative.Vivid[1],
    "XORCache(BL)": px.colors.qualitative.Vivid[1],
    "XOR(BL)+BDI": px.colors.qualitative.Vivid[1],
    "XORCache+BPC(BL)": px.colors.qualitative.Vivid[1],
    "maxbytemap-shuffle-xorfold": px.colors.qualitative.Vivid[1],
    "bytemap-shuffle-xorfold-bpc": px.colors.qualitative.Vivid[1],
    "bytemap-shuffle-xorfold-immo": px.colors.qualitative.Vivid[1],
    "ternarybytemap-shuffle-xorfold": px.colors.qualitative.Vivid[1],

    "lowentropy_8_4": px.colors.qualitative.Vivid[5],
    "lowentropy_8_16(BCD)": px.colors.qualitative.Vivid[5],
    "lowentropy_8_16(BCD)-immo": px.colors.qualitative.Vivid[5],

    "sparseshuffledbytemap_8_7": px.colors.qualitative.Vivid[7],
    "sparse byte labeling-flat": px.colors.qualitative.Vivid[7],
    "XORCache(SBL)": px.colors.qualitative.Vivid[7],
    "XOR(SBL)+BDI": px.colors.qualitative.Vivid[7],
    "XORCache+BPC(SBL)": px.colors.qualitative.Vivid[7],
    #"#2f8ac4", "#3c97d0", "#51a2d5", "#66adda"
    "XORCache(SBL)-1": "#256d9b",
    "XORCache(SBL)-2": "#205f86",
    "XORCache(SBL)-3": "#1b5072",
    "XORCache(SBL)-4": "#16425d",
    "sparseshuffledbytemap_8_4": px.colors.qualitative.Vivid[7],
    "sparseshuffledbytemap_4_3": px.colors.qualitative.Vivid[7],
    "sparseshuffledbytemap_4_3-bpc": px.colors.qualitative.Vivid[7],
    "sparseshuffledbytemap_4_2": px.colors.qualitative.Vivid[7],

    "EPC word labeling-flat": px.colors.qualitative.Vivid[6],
    "EPC word labeling-BPC": px.colors.qualitative.Vivid[6],
    "EPC word labeling": px.colors.qualitative.Vivid[6],
    "strong word labeling": px.colors.qualitative.Vivid[10],
    "strong word labeling-BPC": px.colors.qualitative.Vivid[10],
    "hycomp word labeling-flat": px.colors.qualitative.Vivid[0],
    "hycomp word labeling": px.colors.qualitative.Vivid[0],
    "hycomp word labeling (BPC)": px.colors.qualitative.Vivid[0],
    "semantic word labeling": px.colors.qualitative.Vivid[10],
    "density word labeling": px.colors.qualitative.Vivid[10],
    "density word labeling bpc": px.colors.qualitative.Vivid[10],
    "average byte msb(4) word labeling": px.colors.qualitative.Vivid[10],
    "average byte msb(3) word labeling": px.colors.qualitative.Vivid[10],
    "average byte msb(2) word labeling": px.colors.qualitative.Vivid[10],
    "bdi line labeling": px.colors.qualitative.Vivid[10],
    "bdi-immo line labeling": px.colors.qualitative.Vivid[10],
    "bpc line labeling": px.colors.qualitative.Vivid[10],
}

scheme_to_name = {
    "XORCache+BDI randSet": "randset",
    "XOR randBank+BDI": "randbank",
    "XOR idealSet+BDI": "idealset",
    "XOR idealSet-0+BDI": "idealset",
    "XOR idealSet-1+BDI": "idealset_shift1",
    "XOR idealSet-2+BDI": "idealset_shift2",
    "XOR idealSet-3+BDI": "idealset_shift3",
    "XOR idealSet-4+BDI": "idealset_shift4",
    "XOR idealBank+BDI": "idealbank",
    "oracle": "idealbank",
    # "idealBank": "idealbank_bpc",
    # "oracle": "idealbank_bdi",


    "XORCache+BPC randSet": "randset_bpc",
    "XOR randBank+BPC": "randbank_bpc",
    "XOR idealSet+BPC": "idealset_bpc",
    "XOR idealSet-0+BPC": "idealset_bpc",
    "XOR idealSet-1+BPC": "idealset_bpc_shift1",
    "XOR idealSet-2+BPC": "idealset_bpc_shift2",
    "XOR idealSet-3+BPC": "idealset_bpc_shift3",
    "XOR idealSet-4+BPC": "idealset_bpc_shift4",
    "XOR idealBank+BPC": "idealbank_bpc",

    "bpc": "vanila_bpc",
    "BPC (no XOR)": "vanila_bpc",
    "Bitplane (no XOR)": "vanila_bpc",
    "bdi": "vanila_bdi",
    "BDI (no XOR)": "vanila_bdi",
    "BDI (no XOR) ": "vanila_bdi",
    "BDI": "vanila_bdi_big_end",
    "bdi-immo": "vanila_bdi_allowimmo",
    "thesaurus-flat": "thesaurus",
    "XORCache(LSH-RP)": "thesaurus-banked",
    "XOR(LSH-RP)+BDI": "thesaurus-banked",
    "XORCache+BPC(LSH-RP)": "thesaurus-banked-bpc",
    "thesaurus-immo": "thesaurus-immo",
    "shuffle-xorfold": "fbsxf",
    "bit-sampling-flat": "bs",
    "XORCache(LSH-BS)": "bs-banked",
    "XOR(LSH-BS)+BDI": "bs-banked",
    "XORCache+BPC(LSH-BS)": "bs-banked-bpc",
    "masked-bit-sampling_8_32": "maskedbs_8_32",
    "masked-bit-sampling_4_16": "maskedbs_4_16",
    "masked-bit-sampling_8_16": "maskedbs_8_16",
    "masked-bit-sampling_8_16 (BPC)": "maskedbs_8_16-banked-bpc",
    "masked-bit-sampling_4_8": "maskedbs_4_8",
    "byte labeling-flat": "shuffledbytemap",
    "XORCache(BL)": "shuffledbytemap-banked",
    "XOR(BL)+BDI": "shuffledbytemap-banked",
    "XORCache+BPC(BL)": "shuffledbytemap-banked-bpc",
    "maxbytemap-shuffle-xorfold": "shuffledmaxbytemap",
    "bytemap-shuffle-xorfold-bpc": "shuffledbytemap-bpc",
    "bytemap-shuffle-xorfold-immo": "shuffledbytemap-immo",
    "ternarybytemap-shuffle-xorfold": "shuffledtbytemap",
    "lowentropy_8_4": "lowentropy_8_4",
    "lowentropy_8_16(BCD)": "lowentropy_8_16",
    "lowentropy_8_16(BCD)-immo": "lowentropy_8_16-immo",
    "sparseshuffledbytemap_8_7": "sparseshuffledbytemap_8_7",
    "sparse byte labeling-flat": "sparseshuffledbytemap_8_6",
    "XORCache(SBL)": "sparseshuffledbytemap_8_6-banked",
    "XOR(SBL)+BDI": "sparseshuffledbytemap_8_6-banked",
    "XORCache+BPC(SBL)": "sparseshuffledbytemap_8_6-banked-bpc",
    "XORCache(SBL)-1": "sparseshuffledbytemap_8_6_shift1-banked",
    "XORCache(SBL)-2": "sparseshuffledbytemap_8_6_shift2-banked",
    "XORCache(SBL)-3": "sparseshuffledbytemap_8_6_shift3-banked",
    "XORCache(SBL)-4": "sparseshuffledbytemap_8_6_shift4-banked",
    "sparseshuffledbytemap_8_4": "sparseshuffledbytemap_8_4",
    "sparseshuffledbytemap_4_3": "sparseshuffledbytemap_4_3",
    "sparseshuffledbytemap_4_3-bpc": "sparseshuffledbytemap_4_3-bpc",
    "sparseshuffledbytemap_4_2": "sparseshuffledbytemap_4_2",
    "EPC word labeling-flat": "epc_word_labeling",
    "EPC word labeling": "epc_word_labeling-banked",
    "EPC word labeling-BPC": "epc_word_labeling-banked-bpc",
    "strong word labeling": "strong_word_labeling",
    "strong word labeling-BPC": "strong_word_labeling-banked-bpc",
    "hycomp word labeling-flat": "hycomp_word_labeling",
    "hycomp word labeling": "hycomp_word_labeling-banked",
    "hycomp word labeling (BPC)": "hycomp_word_labeling-banked-bpc",
    "semantic word labeling": "semantic_word_labeling",
    "density word labeling": "density_word_labeling",
    "density word labeling bpc": "density_word_labeling-bpc",
    "average byte msb(4) word labeling": "averagebytemsb_word_labeling_32",
    "average byte msb(3) word labeling": "averagebytemsb_word_labeling_24",
    "average byte msb(2) word labeling": "averagebytemsb_word_labeling_16",
    "bdi line labeling": "bdi_line_labeling_4-banked",
    "bdi-immo line labeling": "bdi_immo_line_labeling_4-banked",
    "bpc line labeling": "bpc_line_labeling_8-banked",
}

stat_to_name = {
    "hamming byte position<br>after oracle": "hamming_byte_position_oracle",
    "hamming byte position<br>after xor bytemap": "hamming_byte_position_afterxor12_bytemap",

    "entropy byte position": "entropy_byte_position",
    "entropy byte position<br>after oracle": "entropy_byte_position_oracle",
    "entropy byte position<br>after xor rand bank": "entropy_byte_position_afterxor_randbank",
    "entropy byte position<br>after xor bytemap": "entropy_byte_position_afterxor12_bytemap",
    "entropy byte position<br>after xor bytemap only xored": "entropy_byte_position_afterxor12_bytemap-onlyxored",
    "entropy byte position<br>after xor sparseshuffledbytemap_4_3": "entropy_byte_position_afterxor12_sparseshuffledbytemap_4_3",
    "entropy byte position<br>after xor sparseshuffledbytemap_4_3 only xored": "entropy_byte_position_afterxor12_sparseshuffledbytemap_4_3-onlyxored",
    "entropy byte position<br>after xor thesaurus": "entropy_byte_position_afterxor12_thesaurus",
    "entropy byte position<br>after xor lowentropy_8_16(BCD)": "entropy_byte_position_afterxor12_lowentropy_8_16",
    "entropy byte position<br>after xor lowentropy_8_16(BCD) only xored": "entropy_byte_position_afterxor12_lowentropy_8_16-onlyxored",
    "histogram word pattern epc": "histogram_word_pattern",
    "histogram word pattern strong": "histogram_word_pattern_strong",
    "histogram word pattern hycomp": "histogram_word_pattern_hycomp",
    "histogram word pattern semantic": "histogram_word_pattern_semantic",
    "histogram word pattern density": "histogram_word_pattern_density",
    "histogram word pattern averagebytemsb 4": "histogram_word_pattern_averagebytemsb_32",
    "histogram word pattern averagebytemsb 3": "histogram_word_pattern_averagebytemsb_24",
    "histogram word pattern averagebytemsb 2": "histogram_word_pattern_averagebytemsb_16",

    "heatcube size change<br>after oracle": "heatcube_bdi_sizechange_oracle",
}

class HashSchemeMaps:
    def __init__(self, schems_to_include):
        self.scheme_to_name = {}
        for key in schems_to_include:
            if key in scheme_to_name:
                self.scheme_to_name[key] = scheme_to_name[key]
            else:
                print("Warning: scheme_to_name does not contain key: " + key)
                assert(False)

        self.crs_schemes = {}
        for key in self.scheme_to_name:
            self.crs_schemes[key] = []
        self.crs_max_schemes = {}
        for key in self.scheme_to_name:
            self.crs_max_schemes[key] = []
        self.crs_min_schemes = {}
        for key in self.scheme_to_name:
            self.crs_min_schemes[key] = []

        self.ers_schemes = {}
        for key in self.scheme_to_name:
            self.ers_schemes[key] = []
        self.ers_max_schemes = {}
        for key in self.scheme_to_name:
            self.ers_max_schemes[key] = []
        self.ers_min_schemes = {}
        for key in self.scheme_to_name:
            self.ers_min_schemes[key] = []
        
        self.frs_schemes = {}
        for key in self.scheme_to_name:
            self.frs_schemes[key] = []
        self.frs_max_schemes = {}
        for key in self.scheme_to_name:
            self.frs_max_schemes[key] = []
        self.frs_min_schemes = {}
        for key in self.scheme_to_name:
            self.frs_min_schemes[key] = []
        
        self.intras_schemes = {}
        for key in self.scheme_to_name:
            self.intras_schemes[key] = []
        self.intras_max_schemes = {}
        for key in self.scheme_to_name:
            self.intras_max_schemes[key] = []
        self.intras_min_schemes = {}
        for key in self.scheme_to_name:
            self.intras_min_schemes[key] = []
        
        self.hammings_schemes = {}
        for key in self.scheme_to_name:
            self.hammings_schemes[key] = []
        self.hammings_max_schemes = {}
        for key in self.scheme_to_name:
            self.hammings_max_schemes[key] = []
        self.hammings_min_schemes = {}
        for key in self.scheme_to_name:
            self.hammings_min_schemes[key] = []
        
    
class ProfileStatMaps:
    def __init__(self, stat_to_include):
        self.stat_to_name = {}
        for key in stat_to_include:
            if key in stat_to_name:
                self.stat_to_name[key] = stat_to_name[key]
            else:
                print("Warning: stat_to_name does not contain key: " + key)
                assert(False)

        self.stat_arr = {}
        for key in self.stat_to_name:
            self.stat_arr[key] = []

import yaml, os, sys
from collections import OrderedDict
from yamlordereddictloader import SafeDumper as safe_dumper_ordered
from yamlordereddictloader import SafeLoader as safe_loader_ordered
class bcolors:
    PURPLE = "\033[95m"
    OKBLUE = "\033[94m"
    OKCYAN = "\033[96m"
    OKGREEN = "\033[92m"
    YELLOW = "\033[93m"
    RED = "\033[91m"
    ENDC = "\033[0m"
    BOLD = "\033[1m"
    UNDERLINE = "\033[4m"
    SKY = "\033[36m"
    REVERSE = "\u001b[7m"
    ENDREVERSE = "\u001b[0m"
    THUMB = "\U0001f44d"
    yellow = "#FFCA68"
    orange = "#FFA668"
    green = "#71DBBA"
    # gray = "#B8B8B8"
    gray = "#9E9E9E"
    blue = "#6D9CF5"
    red = "#FD661F"
    brown = "#9E7446"
    cactus = "#9CC424"
def create_folder(directory):
    """
    Checks the existence of a directory, if does not exist, create a new one
    :param directory: path to directory under concern
    :return: None
    """
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print(
            bcolors.FAIL
            + "ERROR: Creating directory: "
            + directory
            + bcolors.ENDC
        )
        sys.exit()

def yaml_load(file):
    return yaml.load(open(file), Loader=safe_loader_ordered)


def yaml_overwrite(file, content):
    """
    if file exists at filepath, overwite the file, if not, create a new file
    :param filepath: string that specifies the destination file path
    :param content: yaml string that needs to be written to the destination file
    :return: None
    """
    if os.path.exists(file):
        os.remove(file)
    create_folder(os.path.dirname(file))
    out_file = open(file, "a")
    out_file.write(
        yaml.dump(
            content, default_flow_style=False, Dumper=safe_dumper_ordered
        )
    )
