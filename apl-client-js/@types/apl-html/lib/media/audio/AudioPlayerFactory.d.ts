/*!
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { IAudioEventListener } from './IAudioEventListener';
import { IAudioPlayer } from './IAudioPlayer';
export declare abstract class IAudioPlayerFactory {
    abstract tick(): void;
}
export declare type AudioPlayerFactory = (eventListener: IAudioEventListener) => IAudioPlayer;
