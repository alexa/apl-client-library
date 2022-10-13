/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
import { IMediaPlayerHandle } from "./@types/apl-html/lib/index";
import { APLClient } from './APLClient';
import { APLMediaPlayer } from './APLMediaPlayer';
/**
 * Websocket transport AudioPlayer factory.
 */
export declare class APLMediaPlayerFactory implements APL.MediaPlayerFactory {
    private client;
    private playerFactoryFunc;
    private players;
    constructor(client: APLClient, playerFactoryFunc: any);
    static create(client: APLClient, playerFactoryFunc: any): APLMediaPlayerFactory;
    createPlayer(id: string): APLMediaPlayer;
    getMediaPlayer(playerId: string): APLMediaPlayer;
    deleteMediaPlayer(playerId: string): void;
    delete(): void;
}
export declare type MediaPlayerFactoryFunc = (aplMediaPlayer: APL.MediaPlayer) => IMediaPlayerHandle;
