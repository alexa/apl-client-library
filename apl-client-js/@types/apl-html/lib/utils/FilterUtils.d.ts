/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import { IBlend } from '../components/filters/Blend';
import { IBlur } from '../components/filters/Blur';
import { IColor } from '../components/filters/Color';
import { IGradientFilter } from '../components/filters/Gradient';
import { IGrayscale } from '../components/filters/Grayscale';
import { INoise } from '../components/filters/Noise';
import { ISaturate } from '../components/filters/Saturate';
import { FilterType } from '../enums/FilterType';
import { ILogger } from '../logging/ILogger';
/**
 * @ignore
 */
export declare type Filter = IBlur | INoise | IColor | IGradientFilter | IBlend | IGrayscale | ISaturate;
/**
 * The APL filters needs to be handled by SVG filter.
 */
export declare const SVGFilters: Set<FilterType>;
/**
 * Generate SVG definition based on the provided APL filter array.
 * @param filters APL Filters
 * @param imageSrcArray image source array
 * @param filterId filter id
 * @return SVGDefsAndUseElement, if there is no SVGFilter requested in APLFilter, return undefined.
 */
export declare const generateSVGDefsAndUseElement: (filters: Filter[], imageSrcArray: string[], filterId: string) => {
    svgDefsElement: SVGDefsElement;
    svgUseElement: SVGUseElement;
};
/**
 * Generate SVG filter primitive fetches image data from an external source and provides the pixel data as output.
 * @param sourceImageId the src URL from image source array
 * @param filterElement filter element primitive
 * @param isDestinationIn used by blend filter, if true, set as destination in
 * @returns filterImageArray feImage array, store all the SVG feImage
 */
export declare const generateSVGFeImage: (sourceImageId: string, filterElement: SVGElement, isDestinationIn?: boolean) => SVGFEImageElement[];
/**
 * Util function to check whether image source/destination index are out of bound.
 * @param index source or destination image index
 * @param imageArrayLength length of image source array
 * @returns boolean, if true, ignore this filter stage and continue.
 */
export declare const isIndexOutOfBound: (index: number, imageArrayLength: number) => boolean;
export interface SVGImageFiltersApplierArgs {
    uuid: string;
    svgElement: SVGElement;
    imageElement: SVGElement;
    filters: Filter[];
    imageSources: string[];
    logger?: ILogger;
}
export interface SVGImageFiltersApplier {
    applyFiltersToSVGImage: () => void;
}
export declare function createSVGImageFiltersApplier(args: SVGImageFiltersApplierArgs): SVGImageFiltersApplier;
