/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { ElementTypes } from '../components/helpers/StylesApplier';
import { FontStyle } from '../enums/FontStyle';
/**
 * Adjust exports to only export used interfaces
 */
export interface SetArgs {
    element: Element;
    lang: string;
}
export interface SetFontOptions {
    elementType?: ElementTypes;
}
export interface SetFontFamilyArgs extends SetArgs {
    fontFamily: string;
}
export interface SetFontStyleArgs extends SetArgs {
    fontStyle: FontStyle;
}
export interface SetFontWeightArgs extends SetArgs {
    fontWeight: FontWeight;
}
export declare type FontWeight = 'normal' | 'bold' | 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900 | 950;
export declare class FontUtils {
    private static initialized;
    private static initializationCallback;
    private static logger;
    /**
     * Load the fonts using webfontloader
     * @see {@link https://github.com/typekit/webfontloader#custom|webfontloader}
     * @see {@link https://github.com/typekit/fvd|font variation description} for syntax on webfonts
     */
    static initialize(): Promise<void>;
    static getFont(fontFamily: string): string;
    static getFontStyle(fontStyle: number): string;
    static setFontFamily({ element, fontFamily, lang }: SetFontFamilyArgs, options?: SetFontOptions): void;
    static setFontStyle({ element, fontStyle, lang }: SetFontStyleArgs, options?: SetFontOptions): void;
    static setFontWeight({ element, fontWeight, lang }: SetFontWeightArgs, options?: SetFontOptions): void;
}
