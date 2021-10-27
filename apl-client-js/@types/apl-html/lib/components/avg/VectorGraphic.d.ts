/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

import APLRenderer from '../../APLRenderer';
import { PropertyKey } from '../../enums/PropertyKey';
import { VectorGraphicScale } from '../../enums/VectorGraphicScale';
import { ActionableComponent } from '../ActionableComponent';
import { Component, FactoryFunction, IComponentProperties } from '../Component';
import { VectorGraphicElementUpdater } from './VectorGraphicElementUpdater';
export interface IVectorGraphicProperties extends IComponentProperties {
    [PropertyKey.kPropertyGraphic]: APL.Graphic;
    [PropertyKey.kPropertyMediaBounds]: APL.Rect;
    [PropertyKey.kPropertyScale]: VectorGraphicScale;
    [PropertyKey.kPropertySource]: string;
}
/**
 * @ignore
 */
export declare class VectorGraphic extends ActionableComponent<IVectorGraphicProperties> {
    static readonly SVG_NS: string;
    private graphic;
    private svg;
    private vectorGraphicUpdater;
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, vectorGraphicUpdater: VectorGraphicElementUpdater, parent?: Component);
    setProperties(props: IVectorGraphicProperties): Promise<void>;
    private initSvg(root);
    private getInnerBounds();
}
