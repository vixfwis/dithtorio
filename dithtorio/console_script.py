from libdithtorio import process_file, preload_modded, preload_vanilla
import sys
import os
import click

@click.command()
@click.argument('name', type=click.Path(exists=True))
@click.option('-p', '--preset', type=click.Choice(['vanilla', 'modded']), default='modded', show_default=True, help='Palette to use. Modded => Color Coding')
@click.option('-oi', '--output-image', type=click.Path(), default='', help='Output image file')
@click.option('-ob', '--output-bp', type=click.Path(), default='', help='Output blueprint text file')
@click.option('--force-nt', is_flag=True, default=False, show_default=True, help='Force processing as non-transparent image')
@click.option('-t', '--threshold', type=click.IntRange(0, 255), default=0, show_default=True, help='Pixel alpha channel value to be considered transparent. 0 - 255')
@click.option('-s', '--split', type=click.IntRange(1000, 10000000), default=32768, show_default=True, help='Blueprint size (ingame objects). Huge blueprints may slow down the game until they\'re fully loaded. 1 000 - 10 000 000')
def main(name, preset, output_image, output_bp, force_nt, threshold, split):
    """This script converts an image to Factorio blueprint"""
    path, ext = os.path.splitext(name)
    head, tail = os.path.split(path)
    out_img = os.path.join(head, 'converted_'+tail+'.png')
    out_bp = os.path.join(head, 'blueprint_'+tail+'.txt')
    if output_image != '':
        out_img = output_image
    if output_bp != '':
        out_bp = output_bp
    if preset == 'vanilla':
        preload_vanilla()
    elif preset == 'modded':
        preload_modded()
    
    process_file(name, out_img, out_bp, force_nt, threshold, split)
