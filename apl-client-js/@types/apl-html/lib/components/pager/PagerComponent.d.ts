/*!
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import APLRenderer from '../../APLRenderer';
import { PropertyKey } from '../../enums/PropertyKey';
import { ActionableComponent } from '../ActionableComponent';
import { Component, FactoryFunction, IComponentProperties } from '../Component';
/**
 * @ignore
 */
export interface IPagerProperties extends IComponentProperties {
    [PropertyKey.kPropertyInitialPage]: number;
    [PropertyKey.kPropertyCurrentPage]: number;
}
/**
 * @ignore
 */
export declare class PagerComponent extends ActionableComponent<IPagerProperties> {
    constructor(renderer: APLRenderer, component: APL.Component, factory: FactoryFunction, parent?: Component);
    protected isLayout(): boolean;
    /**
     * Returns the page currently being displayed by this component
     * @returns {number} The currently displayed page
     */
    getCoreCurrentPage(): number;
    /**
     * @override
     * @returns The value of the component, as defined by the APL specification
     * @return {any}
     */
    getValue(): any;
}
