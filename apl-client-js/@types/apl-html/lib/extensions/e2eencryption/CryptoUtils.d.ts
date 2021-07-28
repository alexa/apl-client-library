/*
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
export declare enum Algorithm {
    RSASHA1 = "RSA/ECB/OAEPWithSHA1AndMGF1Padding",
    RSASHA256 = "RSA/ECB/OAEPWithSHA256AndMGF1Padding",
    AES = "AES/GCM/NoPadding"
}
export declare enum CryptoHash {
    SHA256 = "SHA-256",
    SHA1 = "SHA-1"
}
export interface EncodeBase64Args {
    token: string;
    value: string;
}
export interface EncryptBase64Args extends EncodeBase64Args {
    algorithm: string;
    key: string;
    aad?: string;
    base64Encoded?: boolean;
}
export interface Base64EncodeResult {
    token: string;
    base64EncodedData: string;
}
export interface RSAEncryptResult {
    token: string;
    base64EncryptedData: string;
}
export interface AESEncryptResult {
    token: string;
    base64EncryptedData: string;
    base64EncodedIV: string;
    base64EncodedKey?: string;
}
export interface EncryptionError {
    token: string;
    errorReason: string;
}
export declare type EncryptResult = RSAEncryptResult | AESEncryptResult | EncryptionError;
export declare type ExtensionResult = EncryptResult | Base64EncodeResult;
export declare function encodeBase64(args: EncodeBase64Args): Base64EncodeResult;
export declare function encrypt(args: EncryptBase64Args): Promise<EncryptResult>;
export declare const testUtils: {
    getArrayBufferFrom: (source: string) => ArrayBuffer;
    getStringFrom: (source: ArrayBuffer) => string;
    getArrayBufferFromBase64: (base64Source: string) => ArrayBuffer;
    getBase64From: (source: ArrayBuffer) => string;
};
