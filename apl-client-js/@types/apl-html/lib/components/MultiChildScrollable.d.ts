/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer from '../APLRenderer';
import { PropertyKey } from '../enums/PropertyKey';
import { ScrollDirection } from '../enums/ScrollDirection';
import { Component, FactoryFunction, IComponentProperties } from './Component';
import { Scrollable } from './Scrollable';
/**
 * @ignore
 */
export interface IMultiChildScrollableProperties extends IComponentProperties {
    [PropertyKey.kPropertyScrollDirection]: ScrollDirection;
    [PropertyKey.kPropertyScrollPosition]: number;
    [PropertyKey.kPropertyNotifyChildrenChanged]: any;
}
/**
 * @ignore
 */
export declare abstract class MultiChildScrollable extends Scrollable<IMultiChildScrollableProperties> {
    protected fullyLoaded: boolean;
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    init(): void;
    protected allowFocus(requestedDistance: number, moveTo: HTMLDivElement): boolean;
    private setScrollPosition;
    private setScrollDirection;
}
