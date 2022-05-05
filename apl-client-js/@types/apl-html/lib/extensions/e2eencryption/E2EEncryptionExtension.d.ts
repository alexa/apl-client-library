/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { CreateAplExtensionCommandDefintion, CreateAplExtensionEventHandler, IExtension } from '../IExtension';
export declare const E2EEncryptionExtensionUri = "aplext:e2eencryption:10";
export declare function createE2EEncryptionExtension(createExtensionCommand: CreateAplExtensionCommandDefintion, createExtensionEventHandler: CreateAplExtensionEventHandler): IExtension;
