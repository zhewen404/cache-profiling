import numpy as np
import plotly.graph_objects as go
import plotly.subplots as sp
import pandas as pd
import plotly.io as pio
pio.kaleido.scope.mathjax = None
import argparse

def calc_map_table(num_tag_entries, hash_fingerprint_size_in_bits, map_table_num_sets, map_table_num_ways):
    # num_data_entries = num_tag_entries / tag_to_data_ratio
    pointer_size_in_bits = np.log2(num_tag_entries)

    num_set_bits = float(np.log2(map_table_num_sets))

    # num_bins = 2**hash_fingerprint_size_in_bits
    map_table_num_entries = map_table_num_sets * map_table_num_ways

    tag_size_in_bits = 0
    replacement_bits = 0

    if (float(np.log2(map_table_num_entries)) > hash_fingerprint_size_in_bits):
        return 0,0,0

    if (num_set_bits < float(hash_fingerprint_size_in_bits)):
        tag_size_in_bits = hash_fingerprint_size_in_bits - np.log2(map_table_num_sets)
        replacement_bits = np.log2(map_table_num_ways)
    elif (num_set_bits == float(hash_fingerprint_size_in_bits)):
        if (map_table_num_ways == 1):
            pass
        else: return 0,0,0
    else:
        return 0,0,0

    map_table_entry_size_in_bits = tag_size_in_bits + replacement_bits + pointer_size_in_bits
    map_table_storage_in_bits = map_table_num_entries * map_table_entry_size_in_bits
    map_table_storage_in_KiB = map_table_storage_in_bits / 8 / 1024

    return map_table_num_entries, map_table_entry_size_in_bits, map_table_storage_in_KiB

if __name__ == "__main__":
    # add argument to specify suitename, launch_flag, and plot
    parser = argparse.ArgumentParser(description='run and plot script')
    parser.add_argument('--fp',
                        help='fingerprint size in bits',
                        default=12)
    parser.add_argument('--tag',
                        help='tag entries',
                        default=1024)
    args = parser.parse_args()

    num_tag_entries = int(args.tag)
    tag_to_data_ratio = 2
    hash_fingerprint_size_in_bits = int(args.fp)

    map_table_num_sets_arr = [16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
    map_table_num_ways_arr = [1, 2, 4, 8, 16]

    # create 2d array of size len(map_table_num_sets_arr) x len(map_table_num_ways_arr)
    map_table_num_entries_arr = np.zeros((len(map_table_num_sets_arr), len(map_table_num_ways_arr)))
    map_table_entry_size_in_bits_arr = np.zeros((len(map_table_num_sets_arr), len(map_table_num_ways_arr)))
    map_table_storage_in_KiB_arr = np.zeros((len(map_table_num_sets_arr), len(map_table_num_ways_arr)))

    i = 0
    for map_table_num_sets in map_table_num_sets_arr:
        j = 0
        for map_table_num_ways in map_table_num_ways_arr:
            map_table_num_entries, map_table_entry_size_in_bits, map_table_storage_in_KiB = \
                calc_map_table(num_tag_entries, hash_fingerprint_size_in_bits, map_table_num_sets, map_table_num_ways)

            map_table_num_entries_arr[i][j] = map_table_num_entries
            map_table_entry_size_in_bits_arr[i][j] = map_table_entry_size_in_bits
            map_table_storage_in_KiB_arr[i][j] = map_table_storage_in_KiB
            j += 1
        i += 1

    fig = sp.make_subplots(rows=1, cols=3, 
                        #    subplot_titles=("Compression Ratio", "Entropy Reduction", "False Positive Rate"),
                           shared_yaxes=False,
                           horizontal_spacing=0.1,
                           vertical_spacing=0,)
    # plot a vector heatmap with min=0 and max=8
    # annotate the heatmap with the data
    fig.add_trace(
        go.Heatmap(
            z=map_table_num_entries_arr,
            x=[str(x) for x in map_table_num_ways_arr],
            y=[str(x) for x in map_table_num_sets_arr],
            text=map_table_num_entries_arr,
            texttemplate="%{text}",
            zmin=0,
            showscale=False,
        ),
        row=1, col=1,
    )
    fig.update_yaxes(title="#map table entries", row=1, col=1, title_standoff = 0)

    fig.add_trace(
        go.Heatmap(
            z=map_table_entry_size_in_bits_arr,
            x=[str(x) for x in map_table_num_ways_arr],
            y=[str(x) for x in map_table_num_sets_arr],
            text=map_table_entry_size_in_bits_arr,
            texttemplate="%{text}",
            zmin=0,
            showscale=False,
        ),
        row=1, col=2,
    )
    fig.update_yaxes(title="map table entry size in bit", row=1, col=2, title_standoff = 0)

    fig.add_trace(
        go.Heatmap(
            z=map_table_storage_in_KiB_arr,
            x=[str(x) for x in map_table_num_ways_arr],
            y=[str(x) for x in map_table_num_sets_arr],
            text=[[f"{i:.2f}" for i in line] for line in map_table_storage_in_KiB_arr],
            texttemplate="%{text}",
            zmin=0,
            showscale=False,
        ),
        row=1, col=3,
    )
    fig.update_yaxes(title="map table size in KiB", row=1, col=3, title_standoff = 0)
    
    dpi = 300
    w=3.3115
    h=1.3
    fig.update_layout(
        template="plotly_white",
        width=w*dpi,
        height=h*dpi,
        paper_bgcolor='rgba(0,0,0,0)',
        title=f"Map Table Size, fingerprint {hash_fingerprint_size_in_bits} bits, {num_tag_entries} tag entirs",
        xaxis_title=None,
        margin=dict(l=5, r=5, t=40, b=20),
        font=dict(
            family="ubuntu",
            size=16,
        ),
        bargap=0.10,
        bargroupgap=0.0,
    )
    # fig.show()
    fig.write_image(f"map_table_size_fp{hash_fingerprint_size_in_bits}b_numtag{num_tag_entries}.pdf")
