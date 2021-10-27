/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer from '../APLRenderer';
import { ImageAlign } from '../enums/ImageAlign';
import { ImageScale } from '../enums/ImageScale';
import { PropertyKey } from '../enums/PropertyKey';
import { Filter } from '../utils/FilterUtils';
import { IGradient } from '../utils/ImageUtils';
import { Component, FactoryFunction, IComponentProperties } from './Component';
/**
 * @ignore
 */
export interface IImageProperties extends IComponentProperties {
    [PropertyKey.kPropertySource]: string | string[];
    [PropertyKey.kPropertyAlign]: ImageAlign;
    [PropertyKey.kPropertyBorderRadius]: number;
    [PropertyKey.kPropertyBorderWidth]: number;
    [PropertyKey.kPropertyOverlayColor]: number;
    [PropertyKey.kPropertyBorderColor]: number;
    [PropertyKey.kPropertyFilters]: Filter[];
    [PropertyKey.kPropertyOverlayGradient]: IGradient;
    [PropertyKey.kPropertyScale]: ImageScale;
}
/**
 * @ignore
 */
export declare class Image extends Component<IImageProperties> {
    private uuid;
    private imageSourcesArray;
    private imageProperties;
    private canvasElement;
    private svgElement;
    private imageSVGElement;
    private imageOverlay;
    private imageView;
    private imageViewProperties;
    private imageOverlayProperties;
    private svgImageElementProperties;
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    init(): void;
    protected boundsUpdated(): void;
    protected onPropertiesUpdated(): void;
    protected applyCssShadow: (shadowParams: string) => void;
    private hasSourceChanged();
    private getSourceArrayFromProperty;
    private draw;
    private fetchSource;
    private renderImage();
    private insertIntoDOM({ element, properties });
    private prepareImageView();
    private prepareImageOverlay();
    private setFilters;
    private setBorderRadius;
    private setOverlayColor;
    private setOverlayGradient;
    private setImageScale;
    private setImageAlignment;
    borderRadius: number;
    borderColor: string;
    borderWidth: number;
    overlayColor: string;
    overlayGradient: string;
    readonly hasNoiseFilter: boolean;
    imageScale: ImageScale;
    imageAlignment: ImageAlign;
    imageFilters: Filter[];
    readonly canvasRenderingContext: CanvasRenderingContext2D;
}
