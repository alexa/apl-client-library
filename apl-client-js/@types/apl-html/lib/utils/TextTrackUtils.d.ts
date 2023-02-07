/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

export declare type TextTrackKind = 'captions' | 'chapters' | 'descriptions' | 'metadata' | 'subtitles';
export declare function clearAllTextTracks(video: HTMLVideoElement): void;
export declare function loadDynamicTextTrack(video: HTMLVideoElement, url: string, kind: TextTrackKind, chunkSize?: number): void;
