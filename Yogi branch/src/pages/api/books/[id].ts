// src/pages/api/books/[id].ts

import type { NextApiRequest, NextApiResponse } from 'next';

// Dummy in-memory data store (replace with DB access layer)
let books: { id: string; [key: string]: any }[] = []; // Replace with proper DB/model in implementation

export default function handler(req: NextApiRequest, res: NextApiResponse) {
  // Validate HTTP method
  if (req.method !== 'DELETE') {
    res.setHeader('Allow', ['DELETE']);
    return res.status(405).json({ message: 'Method Not Allowed' });
  }

  // Extract and validate book id
  const { id } = req.query;
  if (!id || typeof id !== 'string') {
    return res.status(400).json({ message: 'Invalid or missing book ID.' });
  }

  // Remove book from store (replace with actual DB operation)
  const index = books.findIndex((book) => book.id === id);

  if (index === -1) {
    // Book not found
    return res.status(404).json({ message: 'Book not found.' });
  }

  // Delete the book
  books.splice(index, 1);

  // Success response
  return res.status(200).json({ message: 'Book deleted successfully.' });
}