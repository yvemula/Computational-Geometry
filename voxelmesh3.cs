using System;
using System.IO;
using System.Collections.Generic;

class Voxel
{
    public int X;
    public int Y;
    public int Z;
    public byte ColorIndex;

    public Voxel(int x, int y, int z, byte colorIndex)
    {
        X = x;
        Y = y;
        Z = z;
        ColorIndex = colorIndex;
    }

    public override string ToString()
    {
        return $"Voxel [X: {X}, Y: {Y}, Z: {Z}, Color Index: {ColorIndex}]";
    }
}

class VoxFileReader
{
    public static void Main(string[] args)
    {
        string voxelFilePath = "voxel1.vox";

        try
        {
            using (BinaryReader reader = new BinaryReader(File.Open(voxelFilePath, FileMode.Open)))
            {
                // Read header (first 4 bytes should be 'VOX ')
                string magic = new string(reader.ReadChars(4));
                if (magic != "VOX ")
                {
                    throw new Exception("Not a valid .vox file");
                }

                // Read version (4 bytes)
                int version = reader.ReadInt32();

                // Read main chunk
                string mainChunkId = new string(reader.ReadChars(4)); // Should be 'MAIN'
                int mainChunkSize = reader.ReadInt32();
                int mainChildrenSize = reader.ReadInt32();

                List<Voxel> voxels = new List<Voxel>();

                // Parse chunks
                while (reader.BaseStream.Position < reader.BaseStream.Length)
                {
                    string chunkId = new string(reader.ReadChars(4));
                    int chunkSize = reader.ReadInt32();
                    int childChunkSize = reader.ReadInt32();

                    if (chunkId == "SIZE")
                    {
                        // SIZE chunk (contains dimensions of the voxel model)
                        int sizeX = reader.ReadInt32();
                        int sizeY = reader.ReadInt32();
                        int sizeZ = reader.ReadInt32();
                        Console.WriteLine($"Model Size - X: {sizeX}, Y: {sizeY}, Z: {sizeZ}");
                    }
                    else if (chunkId == "XYZI")
                    {
                        // XYZI chunk (contains voxel data)
                        int numVoxels = reader.ReadInt32();
                        Console.WriteLine($"Number of voxels: {numVoxels}");

                        for (int i = 0; i < numVoxels; i++)
                        {
                            int x = reader.ReadByte();
                            int y = reader.ReadByte();
                            int z = reader.ReadByte();
                            byte colorIndex = reader.ReadByte();

                            voxels.Add(new Voxel(x, y, z, colorIndex));
                        }
                    }
                    else
                    {
                        // Skip unknown or irrelevant chunks
                        reader.BaseStream.Seek(chunkSize, SeekOrigin.Current);
                    }
                }

                // Display the voxel data
                foreach (var voxel in voxels)
                {
                    Console.WriteLine(voxel);
                }
            }
        }
        catch (Exception e)
        {
            Console.WriteLine("Error reading .vox file: " + e.Message);
        }
    }
}
