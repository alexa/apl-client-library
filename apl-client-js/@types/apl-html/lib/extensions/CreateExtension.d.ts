/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { CreateAplExtensionCommandDefintion, CreateAplExtensionEventHandler, IExtension } from './IExtension';
export interface CreateExtensionArgs {
    uri: string;
    createAplExtensionCommandDefinition: CreateAplExtensionCommandDefintion;
    createAplExtensionEventHandler: CreateAplExtensionEventHandler;
}
export declare function createExtension(args: CreateExtensionArgs): IExtension;
