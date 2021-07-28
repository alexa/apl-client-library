/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { IAudioEventListener } from './IAudioEventListener';
export declare type AudioPlayerFactory = (eventListener: IAudioEventListener) => AudioPlayer;
export declare type IAudioNode = GainNode;
export declare abstract class AudioPlayer {
    private eventListener;
    private resourceMap;
    private currentSource;
    private decodePromise;
    private static logger;
    private _audioNode;
    constructor(eventListener: IAudioEventListener);
    prepare(url: string, decodeMarkers: boolean): string;
    protected onPlaybackFinished(id: string): void;
    protected onError(id: string, reason: string): void;
    abstract play(id: string): any;
    protected playWithContext(id: string, audioContext: AudioContext): void;
    protected setCurrentAudioNode(node: IAudioNode): void;
    private getAudioNode(context);
    protected disconnectCurrentAudioNode(): void;
    protected cancelPendingAndRemoveCompleted(): void;
    flush(): void;
}
