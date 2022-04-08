/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import APLRenderer from '../APLRenderer';
import { GradientType } from '../enums/GradientType';
import { ImageScale } from '../enums/ImageScale';
import { ILogger } from '../logging/ILogger';
import { IURLRequest } from '../media/IURLRequest';
import { Filter } from './FilterUtils';
export interface IGradient {
    angle: number;
    colorRange: number[];
    inputRange: number[];
    type: GradientType;
}
export declare function getCssGradient(gradient: IGradient, logger: ILogger): string;
export declare function getCssPureColorGradient(color: string): string;
export interface ImageDimensions {
    width: number;
    height: number;
}
export interface ImageScalerArgs {
    imageSource: IURLRequest;
    renderer: APLRenderer;
    scalingOption?: ImageScale;
    imageDimensions: ImageDimensions;
    logger?: ILogger;
}
export interface CanvasImageScalerArgs extends ImageScalerArgs {
    canvas: HTMLCanvasElement;
    filters: Filter[];
    applyFilterArgs: ApplyFiltersToImageArguments;
}
export interface ImageScaler {
    scaleImage: () => ScaledImageSource;
}
export interface ScaledImageSource {
    scaledImageWidth: any;
    scaledImageHeight: any;
    scaledSource: any;
}
export declare function createScaledImageProcessor(args: ImageScalerArgs): Promise<ImageScaler>;
export declare function createCanvasScaledImageProcessor(args: CanvasImageScalerArgs): Promise<ImageScaler>;
export interface CanvasImageFiltersApplierArgs {
    canvas: HTMLCanvasElement;
    canvasRenderingContext: CanvasRenderingContext2D;
    filters: Filter[];
    logger?: ILogger;
}
export interface ApplyFiltersToImageArguments {
    currentImageIndex: number;
    isLastIndex: boolean;
}
export interface CanvasImageFiltersApplier {
    applyFiltersToImage: (args: ApplyFiltersToImageArguments) => void;
}
