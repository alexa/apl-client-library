/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { IMediaPlayerHandle } from "./@types/apl-html/lib/index";
import { APLClient } from './APLClient';
export declare class APLMediaPlayer implements APL.MediaPlayer {
    private client;
    private playerId;
    private mediaPlayerHandle;
    private released;
    constructor(client: APLClient, playerId: string, playerFactory: any);
    delete(): void;
    updateMediaState(mediaState: any): void;
    doCallback(eventType: number): void;
    getMediaPlayerHandle(): IMediaPlayerHandle;
}
